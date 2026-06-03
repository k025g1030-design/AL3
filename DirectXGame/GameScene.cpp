#include "GameScene.hpp"
#include "KamataEngine.h"


namespace Game {
    void GameScene::Initialize() {
        // カメラの初期化
        camera_.Initialize();
        // デバッグカメラの生成
        debugCamera_ = new KamataEngine::DebugCamera(1280, 720);
        SetDebugCameraActive(false);

        if (debugCameraActive_){
            KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
            KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
        }
        

        // skyDomeの初期化
        skyDome_ = new SkyDome();//KamataEngine::Model::CreateFromOBJ("skydome", true);

        // stageData_をもとにBoxを生成
        for (size_t y = 0; y < stageData_.size(); y++) {
            for (size_t x = 0; x < stageData_[y].size(); x++) {
                if (stageData_[y][x] == 1) {
                    Actor::Box* box = new Actor::Box();
                    box->Initialize(KamataEngine::Model::Create(), KamataEngine::TextureManager::Load("images/Wall.png"), &camera_);
                    box->SetPosition({ x * 2.0f, y * 2.0f, 0.0f });
                    boxes_.push_back(box);
                }
            }
        }
        // player initialization should be after boxes initialization to ensure the player is drawn on top of the boxes
        player_ = new Actor::Player();
        player_->Initialize(KamataEngine::Model::CreateFromOBJ("skull"), &camera_);
        player_->SetPosition({ 2.0f, 2.0f, 0.0f });
        
    }

    void GameScene::Update() {

        if (debugCameraActive_) {
            debugCamera_->Update();
        }

        for (auto box : boxes_) {
            box->Update();
        }
        player_->Update();
    }

    void GameScene::Draw() {
        KamataEngine::Model::PreDraw();

        for (auto box : boxes_) {
            if (debugCameraActive_) {
                box->Draw(debugCamera_->GetCamera());
            } else {
                box->Draw(camera_);
            }
        }
        if (debugCameraActive_) {
            player_->Draw(debugCamera_->GetCamera());
        } else {
            player_->Draw();
        }
        KamataEngine::Model::PostDraw();

       
    }
    void GameScene::Finalize() {
        delete debugCamera_;
        delete player_;
        delete skyDome_;

        for (auto box : boxes_) {
            box->Finalize();
            delete box;
        }
        boxes_.clear();
    }
}
