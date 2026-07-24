#include "Player.hpp"
#include "MapChipField.hpp"
#include <algorithm>
#include <array>

namespace Actor {
    void Player::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) {
        assert(model);
        model_ = model;

        // ワールドトランスフォームの初期化
        worldTransform_.Initialize();
        worldTransform_.scale_ = { 2.0f, 2.0f, 2.0f };
        //worldTransform_.scale_ = { 0.06f, 0.06f, 0.06f };
        worldTransform_.rotation_ = { 0, std::numbers::pi_v<float> / 2.0f, 0 };
        worldTransform_.translation_ = position;
        //worldTransform_.rotation_ = { 0, std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> / 2.0f };
    }
    void Player::Update() {
        MoveInput_();

        CollisionMapInfo collisionMapInfo;
        collisionMapInfo.move = velocity_;
        MapCollisionCheck_(collisionMapInfo);
        MoveByCollisionMapInfo_(collisionMapInfo);
        CeilingCollision_(collisionMapInfo);

        UpdateTurn_();
        ApplyTransform_();
    }

    void Player::MoveInput_() {
        KamataEngine::Input* input = KamataEngine::Input::GetInstance();
        bool landing = false;

        if (velocity_.y < 0.0f) {
            if (worldTransform_.translation_.y <= Block::Config::kBlockHeight) {
                landing = true;
            }
        }

        if (onGround_) {
            if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
                KamataEngine::Vector3 acceleration = { 0.0f, 0.0f, 0.0f };
                if (input->PushKey(DIK_A)) {
                    acceleration.x -= World::Config::kAcceleration;
                    if (lrDirection_ != LRDirection::kLeft) {
                        lrDirection_ = LRDirection::kLeft;
                        StartTurn_();
                    }
                }
                if (input->PushKey(DIK_D)) {
                    acceleration.x += World::Config::kAcceleration;
                    if (lrDirection_ != LRDirection::kRight) {
                        lrDirection_ = LRDirection::kRight;
                        StartTurn_();
                    }
                }
                AddVelocity(acceleration);

                // 速度の上限を設定
                velocity_.x = std::clamp(velocity_.x, -World::Config::kLimitRunSpeed, World::Config::kLimitRunSpeed);

            } else {
                // 減速
                velocity_.x *= (1.0f - World::Config::kAttenuation);
            }

            if (input->PushKey(DIK_SPACE)) {
                velocity_ = MathUtils::V3Plus(velocity_, { 0.0f, World::Config::kJumpAcceleration, 0.0f });
                //onGround_ = false;
            }
            if (velocity_.y > 0.0f) {
                onGround_ = false;
            }

        } else {
            /*
            // 空中にいる場合は重力を適用
            velocity_.y -= kGravityAcceleration;
            // 落下速度の上限を設定
            velocity_.y = max(velocity_.y, -kLimitFallSpeed);
            Move();
            */
            velocity_ = MathUtils::V3Plus(velocity_, { 0.0f, -World::Config::kGravityAcceleration, 0.0f });

            // 落下速度の上限を設定
            velocity_.y = max(velocity_.y, -World::Config::kLimitFallSpeed);

            if (landing) {
                worldTransform_.translation_.y = Block::Config::kBlockHeight;
                velocity_.x *= (1.0f - World::Config::kAttenuation);
                velocity_.y = 0.0f;
                onGround_ = true;
            }
        }
    }

    void Player::MapCollisionCheck_(CollisionMapInfo& info) {
        MapCollisionCheckUp_(info);
    }

    void Player::MapCollisionCheckUp_(CollisionMapInfo& info) {
        if (info.move.y <= 0.0f || mapChipField_ == nullptr) {
            return;
        }

        std::array<KamataEngine::Vector3, kNumCorner> positionsNew;
        const KamataEngine::Vector3 centerNew =
            MathUtils::V3Plus(worldTransform_.translation_, info.move);

        for (uint32_t i = 0; i < positionsNew.size(); ++i) {
            positionsNew[i] =
                CornerPosition_(centerNew, static_cast<Corner>(i));
        }

        bool hit = false;
        float allowedMoveY = info.move.y;
        const Corner topCorners[] = { kLeftTop, kRightTop };

        for (Corner corner : topCorners) {
            const Assets::IndexSet indexSet =
                mapChipField_->GetMapChipIndexSetByPosition(positionsNew[corner]);
            const Assets::MapChipType mapChipType =
                mapChipField_->GetMapChipTypeByIndex(
                    indexSet.xIndex, indexSet.yIndex);

            if (mapChipType == Assets::MapChipType::kBlock) {
                const Assets::Rect rect =
                    mapChipField_->GetRectByIndex(
                        indexSet.xIndex, indexSet.yIndex);
                const float moveY =
                    rect.bottom -
                    (worldTransform_.translation_.y + kHeight / 2.0f) -
                    kBlank;
                allowedMoveY = (std::min)(allowedMoveY, (std::max)(0.0f, moveY));
                hit = true;
            }
        }

        if (hit) {
            info.move.y = allowedMoveY;
            info.ceiling = true;
        }
    }

    KamataEngine::Vector3 Player::CornerPosition_(
        const KamataEngine::Vector3& center, Corner corner) const {
        const KamataEngine::Vector3 offsetTable[kNumCorner] = {
            { +kWidth / 2.0f, -kHeight / 2.0f, 0.0f },
            { -kWidth / 2.0f, -kHeight / 2.0f, 0.0f },
            { +kWidth / 2.0f, +kHeight / 2.0f, 0.0f },
            { -kWidth / 2.0f, +kHeight / 2.0f, 0.0f },
        };

        return MathUtils::V3Plus(center, offsetTable[corner]);
    }

    void Player::MoveByCollisionMapInfo_(const CollisionMapInfo& info) {
        worldTransform_.translation_ =
            MathUtils::V3Plus(worldTransform_.translation_, info.move);
    }

    void Player::CeilingCollision_(const CollisionMapInfo& info) {
        if (info.ceiling) {
            KamataEngine::DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
            velocity_.y = 0.0f;
        }
    }
   
    void Player::Draw(const KamataEngine::Camera* camera) {

        model_->Draw(worldTransform_, *camera);
    }
    void Player::Finalize() {
        model_ = nullptr;
    }
}
