#pragma once
#include "KamataEngine.h"

namespace Actor {
    class Player {
    public:
        void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);
        void Update();
        void Draw();
        void Finalize();

    private:
        KamataEngine::Model* model_ = nullptr;
        KamataEngine::WorldTransform worldTransform_;
        KamataEngine::Camera* camera_ = nullptr;

        uint32_t textureHandle_ = 0;

    };
}


