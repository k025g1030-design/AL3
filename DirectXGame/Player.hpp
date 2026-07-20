#pragma once
#include "KamataEngine.h"
#include "Math.hpp"
#include <numbers>
#include "GameConfig.hpp"

namespace Actor {

    enum class LRDirection {
        kRight = 0,
        kLeft = 1,
    };

    class Player {
    public:
        void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position);
        void Update();
        void Draw(const KamataEngine::Camera* camera);
        void Finalize();

    public:
        KamataEngine::WorldTransform& GetWorldTransform() {
            return worldTransform_;
        }

        void SetPosition(const KamataEngine::Vector3& position) {
            worldTransform_.translation_ = position;
        }
        void SetVelocity(const KamataEngine::Vector3& velocity) {
            velocity_ = velocity;
        }
        void AddVelocity(const KamataEngine::Vector3 velocity) {
            velocity_ = MathUtils::V3Plus(velocity_, velocity);
        }
        void Move() {
            worldTransform_.translation_ = MathUtils::V3Plus(worldTransform_.translation_, velocity_);
        }
        

    private:
        bool IsTurning_() const {
            return turnTimer_ > 0.0f;
        }
        void StartTurn_() {
            turnFirstRotationY_ = worldTransform_.rotation_.y;
            turnTimer_ = World::Config::kTimeTurn;
        }

        void UpdateTurn_() {
            if (turnTimer_ > 0.0f) {
                turnTimer_ -= 1 / 60.0f; // Assuming 60 FPS, adjust as needed
                if (turnTimer_ < 0.0f) {
                    turnTimer_ = 0.0f;
                }
                float destinationRotationYTable[] = {
                    std::numbers::pi_v<float> / 2.0f, // 右
                    std::numbers::pi_v<float> *3.0f / 2.0f, // 左
                };

                float destinationRotationY = destinationRotationYTable[static_cast<int>(lrDirection_)];
                worldTransform_.rotation_.y = turnFirstRotationY_ + (destinationRotationY - turnFirstRotationY_) * EaseInOutBounce_(1.0f - turnTimer_ / World::Config::kTimeTurn);
            }

        }

        float EaseOutBounce_(float t) {
            if (t < 1 / 2.75f) {
                return 7.5625f * t * t;
            } else if (t < 2 / 2.75f) {
                t -= 1.5f / 2.75f;
                return 7.5625f * t * t + 0.75f;
            } else if (t < 2.5f / 2.75f) {
                t -= 2.25f / 2.75f;
                return 7.5625f * t * t + 0.9375f;
            } else {
                t -= 2.625f / 2.75f;
                return 7.5625f * t * t + 0.984375f;
            }
        }

        float EaseInOutBounce_(float t) {
            return t < 0.5f
                ? (1 - EaseOutBounce_(1 - 2 * t)) / 2
                : (1 + EaseOutBounce_(2 * t - 1)) / 2;
        }
          

        void ApplyTransform_() {
            worldTransform_.matWorld_ = MathUtils::MakeAffineMatrix(
                worldTransform_.scale_,
                worldTransform_.rotation_,
                worldTransform_.translation_
            );
            worldTransform_.TransferMatrix();
        }

    private:
        KamataEngine::Model* model_ = nullptr;
        KamataEngine::WorldTransform worldTransform_;

        KamataEngine::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
        LRDirection lrDirection_ = LRDirection::kRight;

        float turnFirstRotationY_ = 0.0f;
        float turnTimer_ = 0.0f;
        bool onGround_ = true;



    };
}


