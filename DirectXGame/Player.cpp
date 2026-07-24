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
        if (isDead_) {
            UpdateDeath_();
            ApplyTransform_();
            return;
        }

        MoveInput_();

        CollisionMapInfo collisionMapInfo;
        collisionMapInfo.move = velocity_;
        MapCollisionCheck_(collisionMapInfo);
        MoveByCollisionMapInfo_(collisionMapInfo);
        CeilingCollision_(collisionMapInfo);
        LandingCollision_(collisionMapInfo);
        WallCollision_(collisionMapInfo);

        UpdateTurn_();
        ApplyTransform_();
    }

    void Player::MoveInput_() {
        KamataEngine::Input* input = KamataEngine::Input::GetInstance();
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

            if (input->TriggerKey(DIK_SPACE)) {
                velocity_ = MathUtils::V3Plus(velocity_, { 0.0f, World::Config::kJumpAcceleration, 0.0f });
                onGround_ = false;
            }
            if (onGround_) {
                // 足元の床を毎フレーム確認できるよう、わずかに下向きへ移動させる。
                velocity_.y = -World::Config::kGravityAcceleration;
            }

        } else {
            velocity_ = MathUtils::V3Plus(velocity_, { 0.0f, -World::Config::kGravityAcceleration, 0.0f });
            velocity_.y = max(velocity_.y, -World::Config::kLimitFallSpeed);
        }
    }

    void Player::MapCollisionCheck_(CollisionMapInfo& info) {
        MapCollisionCheckUp_(info);
        MapCollisionCheckDown_(info);
        MapCollisionCheckRight_(info);
        MapCollisionCheckLeft_(info);
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

    void Player::MapCollisionCheckDown_(CollisionMapInfo& info) {
        if (info.move.y >= 0.0f || mapChipField_ == nullptr) {
            return;
        }

        const KamataEngine::Vector3 centerNew =
            MathUtils::V3Plus(worldTransform_.translation_, info.move);
        const Corner bottomCorners[] = { kLeftBottom, kRightBottom };
        float allowedMoveY = info.move.y;
        bool hit = false;

        for (Corner corner : bottomCorners) {
            const Assets::IndexSet indexSet =
                mapChipField_->GetMapChipIndexSetByPosition(
                    CornerPosition_(centerNew, corner));
            if (mapChipField_->GetMapChipTypeByIndex(
                indexSet.xIndex, indexSet.yIndex) == Assets::MapChipType::kBlock) {
                const Assets::Rect rect =
                    mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
                const float moveY =
                    rect.top -
                    (worldTransform_.translation_.y - kHeight / 2.0f) +
                    kBlank;
                allowedMoveY = (std::max)(allowedMoveY, (std::min)(0.0f, moveY));
                hit = true;
            }
        }

        if (hit) {
            info.move.y = allowedMoveY;
            info.landing = true;
        }
    }

    void Player::MapCollisionCheckRight_(CollisionMapInfo& info) {
        if (info.move.x <= 0.0f || mapChipField_ == nullptr) {
            return;
        }

        const KamataEngine::Vector3 centerNew =
            MathUtils::V3Plus(worldTransform_.translation_, info.move);
        const Corner rightCorners[] = { kRightBottom, kRightTop };
        float allowedMoveX = info.move.x;
        bool hit = false;

        for (Corner corner : rightCorners) {
            const Assets::IndexSet indexSet =
                mapChipField_->GetMapChipIndexSetByPosition(
                    CornerPosition_(centerNew, corner));
            if (mapChipField_->GetMapChipTypeByIndex(
                indexSet.xIndex, indexSet.yIndex) == Assets::MapChipType::kBlock) {
                const Assets::Rect rect =
                    mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
                const float moveX =
                    rect.left -
                    (worldTransform_.translation_.x + kWidth / 2.0f) -
                    kBlank;
                allowedMoveX = (std::min)(allowedMoveX, (std::max)(0.0f, moveX));
                hit = true;
            }
        }

        if (hit) {
            info.move.x = allowedMoveX;
            info.hitWall = true;
        }
    }

    void Player::MapCollisionCheckLeft_(CollisionMapInfo& info) {
        if (info.move.x >= 0.0f || mapChipField_ == nullptr) {
            return;
        }

        const KamataEngine::Vector3 centerNew =
            MathUtils::V3Plus(worldTransform_.translation_, info.move);
        const Corner leftCorners[] = { kLeftBottom, kLeftTop };
        float allowedMoveX = info.move.x;
        bool hit = false;

        for (Corner corner : leftCorners) {
            const Assets::IndexSet indexSet =
                mapChipField_->GetMapChipIndexSetByPosition(
                    CornerPosition_(centerNew, corner));
            if (mapChipField_->GetMapChipTypeByIndex(
                indexSet.xIndex, indexSet.yIndex) == Assets::MapChipType::kBlock) {
                const Assets::Rect rect =
                    mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
                const float moveX =
                    rect.right -
                    (worldTransform_.translation_.x - kWidth / 2.0f) +
                    kBlank;
                allowedMoveX = (std::max)(allowedMoveX, (std::min)(0.0f, moveX));
                hit = true;
            }
        }

        if (hit) {
            info.move.x = allowedMoveX;
            info.hitWall = true;
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

    void Player::LandingCollision_(const CollisionMapInfo& info) {
        if (info.landing) {
            velocity_.y = 0.0f;
            onGround_ = true;
        } else if (velocity_.y < 0.0f) {
            onGround_ = false;
        }
    }

    void Player::WallCollision_(const CollisionMapInfo& info) {
        if (info.hitWall) {
            velocity_.x = 0.0f;
        }
    }

    void Player::ConstrainToCamera(
        float left, float right, float bottom, float top) {
        if (isDead_) {
            return;
        }

        const float minimumX = left + kWidth / 2.0f;
        const float maximumX = right - kWidth / 2.0f;
        const float minimumY = bottom + kHeight / 2.0f;
        const float maximumY = top - kHeight / 2.0f;
        const KamataEngine::Vector3 positionBefore = worldTransform_.translation_;

        worldTransform_.translation_.x =
            std::clamp(worldTransform_.translation_.x, minimumX, maximumX);
        worldTransform_.translation_.y =
            std::clamp(worldTransform_.translation_.y, minimumY, maximumY);

        const bool pushed =
            positionBefore.x != worldTransform_.translation_.x ||
            positionBefore.y != worldTransform_.translation_.y;
        if (pushed && IsOverlappingBlock_()) {
            StartDeath_();
        }
        ApplyTransform_();
    }

    bool Player::IsOverlappingBlock_() const {
        if (mapChipField_ == nullptr) {
            return false;
        }

        const float playerLeft = worldTransform_.translation_.x - kWidth / 2.0f;
        const float playerRight = worldTransform_.translation_.x + kWidth / 2.0f;
        const float playerBottom = worldTransform_.translation_.y - kHeight / 2.0f;
        const float playerTop = worldTransform_.translation_.y + kHeight / 2.0f;

        for (uint32_t y = 0; y < mapChipField_->GetNumBlockVertical(); ++y) {
            for (uint32_t x = 0; x < mapChipField_->GetNumBlockHorizontal(); ++x) {
                if (mapChipField_->GetMapChipTypeByIndex(x, y) !=
                    Assets::MapChipType::kBlock) {
                    continue;
                }
                const Assets::Rect rect = mapChipField_->GetRectByIndex(x, y);
                if (playerRight > rect.left + kBlank &&
                    playerLeft < rect.right - kBlank &&
                    playerTop > rect.bottom + kBlank &&
                    playerBottom < rect.top - kBlank) {
                    return true;
                }
            }
        }
        return false;
    }

    void Player::StartDeath_() {
        isDead_ = true;
        velocity_ = {};
    }

    void Player::UpdateDeath_() {
        worldTransform_.rotation_.z += World::Config::kDeathRotationSpeed;
        const float nextScale =
            (std::max)(0.0f, worldTransform_.scale_.x - World::Config::kDeathShrinkSpeed);
        worldTransform_.scale_ = { nextScale, nextScale, nextScale };
        worldTransform_.translation_.y -= World::Config::kDeathFallSpeed;
    }
   
    void Player::Draw(const KamataEngine::Camera* camera) {

        model_->Draw(worldTransform_, *camera);
    }
    void Player::Finalize() {
        model_ = nullptr;
    }
}
