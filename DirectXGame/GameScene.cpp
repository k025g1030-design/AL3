#include "GameScene.hpp"


namespace Game {
    void GameScene::Initialize() {
        textureHandle_ = KamataEngine::TextureManager::Load("uvChecker.png");
        soundDataHandle_ = KamataEngine::Audio::GetInstance()->LoadWave("fanfare.wav");
        

        // モデルの生成
        model_ = KamataEngine::Model::Create();
        // ワールドトランスフォームの初期化
        worldTransform_.Initialize();
        worldTransform_.scale_ = { 0.1f, 0.1f, 0.1f };
        worldTransform_.rotation_ = { 0.0f, 0.0f, 0.0f };
        worldTransform_.translation_ = { 0.0f, 0.0f, 50.0f };
        // カメラの初期化
        camera_.Initialize();
        // デバッグカメラの生成
        debugCamera_ = new KamataEngine::DebugCamera(1280, 720);

        //voiceHandle_ = KamataEngine::Audio::GetInstance()->PlayWave(soundDataHandle_, true);

        KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
        KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
    }

    void GameScene::Update() {
        if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
            //if (KamataEngine::Audio::GetInstance()->IsPlaying(voiceHandle_)) {
            //    KamataEngine::Audio::GetInstance()->StopWave(voiceHandle_);
            //} else {
            //    voiceHandle_ = KamataEngine::Audio::GetInstance()->PlayWave(soundDataHandle_, true);
            //}
            delete debugCamera_;
            debugCamera_ = new KamataEngine::DebugCamera(1280, 720);
            KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
        }
        debugCamera_->Update();
    }

    void GameScene::Draw() {
        KamataEngine::Model::PreDraw();

        

        model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);

        KamataEngine::Model::PostDraw();
    }
    void GameScene::Finalize() {
        KamataEngine::TextureManager::Unload(textureHandle_);
        delete model_;
        delete debugCamera_;
    }
}
