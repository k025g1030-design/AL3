#include "GameScene.hpp"
#include "KamataEngine.h"


namespace Game {
    void GameScene::Initialize() {
        // カメラの初期化
        camera_.Initialize();
        // デバッグカメラの生成
        debugCamera_ = new KamataEngine::DebugCamera(1280, 720);
        SetDebugCameraActive(true);

        if (debugCameraActive_){
            KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
            KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
        }
        

        // skyDomeの初期化
        skyDome_ = new Game::SkyDome();//KamataEngine::Model::CreateFromOBJ("skydome", true);
        skyDome_->Initialize(KamataEngine::Model::CreateFromOBJ("SkyDome", true), &camera_);

        mapChipField_.LoadData();

        GenerateBlocks();

        // player initialization should be after boxes initialization to ensure the player is drawn on top of the boxes
        GeneratePlayer();
        
    }

    void GameScene::Update() {

        if (debugCameraActive_) {
            debugCamera_->Update();
        }

        for (auto box : mapChipField_.GetBlocks()) {
            box->Update();
        }
        player_->Update();
        skyDome_->Update();
    }

    void GameScene::Draw() {
        KamataEngine::Model::PreDraw();

        for (auto box : mapChipField_.GetBlocks()) {
            if (debugCameraActive_) {
                box->Draw(debugCamera_->GetCamera());
            } else {
                box->Draw(camera_);
            }
        }
        if (debugCameraActive_) {
            player_->Draw(debugCamera_->GetCamera());
            skyDome_->Draw(debugCamera_->GetCamera());
        } else {
            player_->Draw();
            skyDome_->Draw();
        }
        KamataEngine::Model::PostDraw();

       
    }
    void GameScene::Finalize() {
        delete debugCamera_;
        delete player_;
        delete skyDome_;

        mapChipField_.Finalize();
    }
}
