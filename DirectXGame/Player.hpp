#pragma once
#include "KamataEngine.h"
#include "Math.hpp"
#include <numbers>
#include "GameConfig.hpp"

namespace Assets {
    class MapChipField;
    struct IndexSet;
}

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
        void ConstrainToCamera(float left, float right, float bottom, float top);
        bool IsDead() const {
            return isDead_;
        }
        void SetMapChipField(Assets::MapChipField* mapChipField) {
            mapChipField_ = mapChipField;
        }

    public:
        KamataEngine::WorldTransform& GetWorldTransform() {
            return worldTransform_;
        }

        KamataEngine::Vector3 GetVelocity() const {
            return velocity_;
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
    private:
        struct CollisionMapInfo {
            bool ceiling = false;
            bool landing = false;
            bool hitWall = false;
            KamataEngine::Vector3 move = { 0.0f, 0.0f, 0.0f };
        };

        enum Corner {
            kRightBottom,
            kLeftBottom,
            kRightTop,
            kLeftTop,
            kNumCorner,
        };

        void MoveInput_();
        void MapCollisionCheck_(CollisionMapInfo& info);
        void MapCollisionCheckUp_(CollisionMapInfo& info);
        void MapCollisionCheckDown_(CollisionMapInfo& info);
        void MapCollisionCheckLeft_(CollisionMapInfo& info);
        void MapCollisionCheckRight_(CollisionMapInfo& info);
        void ResolveMapCollision_(CollisionMapInfo& info);
        bool IsBlock_(const Assets::IndexSet& indexSet) const;
        KamataEngine::Vector3 CornerPosition_(
            const KamataEngine::Vector3& center, Corner corner) const;
        void MoveByCollisionMapInfo_(const CollisionMapInfo& info);
        void CeilingCollision_(const CollisionMapInfo& info);
        void LandingCollision_(const CollisionMapInfo& info);
        void WallCollision_(const CollisionMapInfo& info);
        bool IsOverlappingBlock_() const;
        void StartDeath_();
        void UpdateDeath_();

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
        Assets::MapChipField* mapChipField_ = nullptr;

        float turnFirstRotationY_ = 0.0f;
        float turnTimer_ = 0.0f;
        bool onGround_ = true;
        bool isDead_ = false;

        static inline const float kWidth = 0.8f;
        static inline const float kHeight = 0.8f;
        static inline const float kBlank = 0.01f;
        static inline const float kAttenuationLanding = 0.2f;
        static inline const float kAttenuationWall = 0.2f;
    };
}
