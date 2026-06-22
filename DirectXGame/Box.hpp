#pragma once
#include "KamataEngine.h"
#include "Math.hpp"

namespace Assets {


    class Box {
    public:
        void Initialize(KamataEngine::Model* model, uint32_t textureHandle, const KamataEngine::Vector3& position);
        void Update();
        void Draw(const KamataEngine::Camera* camera);
        void Finalize();

    public:
        void SetPosition(const KamataEngine::Vector3& position) {
            worldTransform_.translation_ = position;
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

        uint32_t textureHandle_ = 0;

    };

}