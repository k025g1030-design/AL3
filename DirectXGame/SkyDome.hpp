#pragma once
#include "KamataEngine.h"
#include "Math.hpp"

namespace Game {
    class SkyDome {
    public:
        void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
        void Update();
        void Draw();
        void Draw(const KamataEngine::Camera& camera);
        void Finalize();


    private:
        KamataEngine::Model* model_ = nullptr;
        KamataEngine::WorldTransform worldTransform_;
        KamataEngine::Camera* camera_ = nullptr;
    };

}

