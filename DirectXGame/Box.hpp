#pragma once
#include "KamataEngine.h"

namespace Actor {
    class Box {
    public:
        void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);
        void Update();
        void Draw();
        void Draw(const KamataEngine::Camera& camera);
        void Finalize();

    public:
        void SetPosition(const KamataEngine::Vector3& position) {
            worldTransform_.translation_ = position;
        }


    private:
        KamataEngine::Model* model_ = nullptr;
        KamataEngine::WorldTransform worldTransform_;
        KamataEngine::Camera* camera_ = nullptr;

        uint32_t textureHandle_ = 0;

    };

}