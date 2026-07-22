#pragma once
#include "KamataEngine.h"
#include "Math.hpp"
#include <algorithm>

namespace Actor {
    class Player;
}

namespace Assets {
    class MapChipField;
}

namespace Game {



    struct Rect {
        float left;
        float top;
        float right;
        float bottom;
    };
    //static inline const Rect kScreenRect = { 0.0f, 0.0f, 1280.0f, 720.0f };


    class CameraController {
    public:
        void Initialize();
        void Update();
        void Reset();
        void ConstrainSideScrollCamera();


    public:
        void SetTarget(Actor::Player* target) {
            target_ = target;
        }
        void SetMapField(Assets::MapChipField* mapField) {
            mapField_ = mapField;
        }

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

        KamataEngine::Vector3 GetVelocity() const {
            return velocity_;
        }
        

        

    private:
        void SetDebugCameraActive_(bool active) {
            debugCameraActive_ = active;
        }

        float ClampAxis(float target, float mapMin, float mapMax, float visibleHalfSize) {
            const float mapSize =
                mapMax - mapMin;

            const float visibleSize =
                visibleHalfSize * 2.0f;

            // 理論上 Z 已經限制過，通常不會進入這裡。
            // 保留這個判斷以防浮點誤差或異常資料。
            if (visibleSize >= mapSize)
            {
                return (mapMin + mapMax) * 0.5f;
            }

            return std::clamp(
                target,
                mapMin + visibleHalfSize,
                mapMax - visibleHalfSize
            );
        }

        



    private:
        KamataEngine::DebugCamera* debugCamera_ = nullptr;
        KamataEngine::Camera camera_;


        // The target player that the camera should follow
        Actor::Player* target_ = nullptr;
        Assets::MapChipField* mapField_ = nullptr;
        KamataEngine::Vector3 targetOffset_ = { 0, 0, -50.0f };

        KamataEngine::Vector3 velocity_{};
    


        bool debugCameraActive_ = false;
    };
}

