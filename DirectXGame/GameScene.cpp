#include "GameScene.hpp"
#include "KamataEngine.h"


namespace Game {
    void GameScene::Initialize() {
        // カメラの初期化
        camera_.Initialize();
        // デバッグカメラの生成
        debugCamera_ = new KamataEngine::DebugCamera(1280, 720);
        KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
        KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());


        //player_ = new Actor::Player();
        //player_->Initialize(KamataEngine::Model::Create(), KamataEngine::TextureManager::Load("uvChecker.png"), &camera_);
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

        
    }

    void GameScene::Update() {
        debugCamera_->Update();

        for (auto box : boxes_) {
            box->Update();
        }
    }

    void GameScene::Draw() {
        KamataEngine::Model::PreDraw();

        for (auto box : boxes_) {
            box->Draw(debugCamera_->GetCamera());
        }

        KamataEngine::Model::PostDraw();

       
    }
    void GameScene::Finalize() {
        delete debugCamera_;


        for (auto box : boxes_) {
            box->Finalize();
            delete box;
        }
        boxes_.clear();
    }
}
