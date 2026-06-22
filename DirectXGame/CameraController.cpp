#include "CameraController.hpp"

namespace Game {
    void CameraController::Initialize() {
        // カメラの初期化
        camera_.Initialize();
        // デバッグカメラの生成
        debugCamera_ = new KamataEngine::DebugCamera(1280, 720);
        SetDebugCameraActive(true);

        if (debugCameraActive_) {
            KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
            KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
        }
    }
    void CameraController::Update() {
        if (debugCamera_) {
            debugCamera_->Update();
        }
    }
}