#include "CameraController.hpp"
#include "Player.hpp"
#include "MapChipField.hpp"

namespace Game {
    void CameraController::Initialize() {
        // カメラの初期化
        camera_.Initialize();
        // デバッグカメラの生成
        debugCamera_ = new KamataEngine::DebugCamera(1280, 720);
        SetDebugCameraActive_(false);

        if (debugCameraActive_) {
            KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
            KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
        }
    }
    void CameraController::Update() {
        if (debugCamera_) {
            debugCamera_->Update();
        }
        ConstrainSideScrollCamera();
        camera_.UpdateMatrix();
        //Reset();
        //ConstrainSideScrollCamera_(
        //    camera_,
        //    0.0f, // Assuming the map plane is at Z=0
        //    5.0f, // Minimum distance from the map plane
        //    10.0f // Safety margin
        //);
    }

    void CameraController::ConstrainSideScrollCamera() {
        const float fovAngleY_ = camera_.fovAngleY;
        const float aspectRatio_ = camera_.aspectRatio;
        const float desiredDistance = std::abs(camera_.translation_.z);
        const float tanHalfFov =
            std::tan(fovAngleY_ * 0.5f);

        if (tanHalfFov <= 0.0f ||
            aspectRatio_ <= 0.0f) {
            return;
        }


        const float mapWidth = mapField_->GetMapChipWidth();

        const float mapHeight = mapField_->GetMapChipHeight();

        if (mapWidth <= 0.0f || mapHeight <= 0.0f) {
            return;
        }

        // --------------------------------------------
        // 1. 計算 Camera 最遠能離地圖多遠
        // --------------------------------------------

        const float maxDistanceByWidth =
            (mapWidth * 0.5f) /
            (tanHalfFov * aspectRatio_);

        const float maxDistanceByHeight =
            (mapHeight * 0.5f) /
            tanHalfFov;

        // 寬和高都不能超過地圖。
        const float maximumDistance =
            min(
                maxDistanceByWidth,
                maxDistanceByHeight
            );

        // 防止 Camera 太接近地圖平面。
        const float validMinimumDistance =
            min(1.0f, maximumDistance);

        const float actualDistance =
            std::clamp(
                desiredDistance,
                validMinimumDistance,
                maximumDistance
            );

        // Camera 位於負 Z，朝向 Z = 0 的地圖。
        camera_.translation_.z = 0 - actualDistance;

        // --------------------------------------------
        // 2. 計算目前 Camera 在地圖平面上的視野大小
        // --------------------------------------------

        const float visibleHalfHeight =
            actualDistance * tanHalfFov;

        const float visibleHalfWidth =
            visibleHalfHeight * aspectRatio_;

        // --------------------------------------------
        // 3. 限制 Camera 的 X/Y 中心位置
        // --------------------------------------------

        camera_.translation_.x = ClampAxis(
            target_->GetWorldTransform().translation_.x,
            0,
            mapWidth,
            visibleHalfWidth
        );

        camera_.translation_.y = ClampAxis(
            target_->GetWorldTransform().translation_.y,
            0,
            mapHeight,
            visibleHalfHeight
        );
    }

    void CameraController::Reset() {
        KamataEngine::WorldTransform& targetWorldTransform = target_->GetWorldTransform();
        camera_.translation_ = MathUtils::V3Plus(targetWorldTransform.translation_, targetOffset_);   
        
        //camera_.farZ = 200.0f;
        camera_.UpdateMatrix();
    }
}