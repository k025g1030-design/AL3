#include "Player.hpp"
#include <algorithm>

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
            UpdateTurn_();

            
            
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
        
        // turnchange中は移動しない
        if (!IsTurning_()) {
            Move();
        }

        ApplyTransform_();
    }
   
    void Player::Draw(const KamataEngine::Camera* camera) {

        model_->Draw(worldTransform_, *camera);
    }
    void Player::Finalize() {
        model_ = nullptr;
    }
}
