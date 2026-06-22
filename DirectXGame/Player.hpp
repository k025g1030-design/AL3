#pragma once
#include "KamataEngine.h"
#include "Math.hpp"

namespace Actor {
    static inline const float kPlayerWidth = 2.0f;
    static inline const float kPlayerHeight = 2.0f;
    static inline const float kAcceleration = 0.1f;

    class Player {
    public:
        void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
        void Update();
        void Draw();
        void Draw(const KamataEngine::Camera& camera);
        void Finalize();

    public:
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
        KamataEngine::Camera* camera_ = nullptr;

        KamataEngine::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };


    };
}


