#include "GameScene.hpp"


namespace Game {
    void GameScene::Initialize() {
        // カメラの初期化
        camera_.Initialize();
        // デバッグカメラの生成
        //debugCamera_ = new KamataEngine::DebugCamera(1280, 720);

        player_ = new Actor::Player();
        player_->Initialize(KamataEngine::Model::Create(), KamataEngine::TextureManager::Load("uvChecker.png"), &camera_);

        //KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
        //KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
        
    }

    void GameScene::Update() {
        player_->Update();
    }

    void GameScene::Draw() {
        KamataEngine::Model::PreDraw();

        player_->Draw();

        KamataEngine::Model::PostDraw();

       
    }
    void GameScene::Finalize() {


        player_->Finalize();
        delete player_;
        player_ = nullptr;
    }
}
