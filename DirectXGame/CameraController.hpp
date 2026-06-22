#pragma once
#include "KamataEngine.h"

namespace Game {
    class CameraController {
    public:
        void Initialize();
        void Update();

    public:
        KamataEngine::Camera* GetCamera() {
            // If debug camera is active, return pointer to its internal Camera instance.
            // DebugCamera::GetCamera() returns a const Camera&; take its address and remove
            // constness because the underlying Camera is owned by DebugCamera and lives
            // as long as debugCamera_ exists.
            if (debugCameraActive_ && debugCamera_) {
                return const_cast<KamataEngine::Camera*>(&debugCamera_->GetCamera());                   
            }
            return &camera_;
        }

    private:
        void SetDebugCameraActive(bool active) {
            debugCameraActive_ = active;
        }

    private:
        KamataEngine::DebugCamera* debugCamera_ = nullptr;
        KamataEngine::Camera camera_;

        bool debugCameraActive_ = false;
    };
}

