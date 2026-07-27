#include "GameScene.hpp"
#include "KamataEngine.h"


namespace Game {
    void GameScene::Initialize() {
        cameraController_.Initialize();

        // skyDomeの初期化
        skyDome_ = new Game::SkyDome();//KamataEngine::Model::CreateFromOBJ("skydome", true);
        skyDome_->Initialize(KamataEngine::Model::CreateFromOBJ("SkyDome", true), cameraController_.GetCamera());

        GenerateMapData();
        GenerateBlocks();

        // player initialization should be after boxes initialization to ensure the player is drawn on top of the boxes

        GeneratePlayer();
        player_->SetMapChipField(currentMap_);
        GenerateEnemies();

        cameraController_.SetTarget(player_);
        cameraController_.SetMapField(currentMap_);
        cameraController_.ConstrainSideScrollCamera();
        cameraController_.GetCamera()->UpdateMatrix();

        
    }

    void GameScene::Update() {
        KamataEngine::Input* input = KamataEngine::Input::GetInstance();
        if (!player_->IsDead() && input->TriggerKey(DIK_P)) {
            cameraController_.SetMode(CameraController::Mode::kForcedScroll);
        }

        cameraController_.Update();
        

        for (auto box : currentMap_->GetBlocks()) {
            box->Update();
        }
        player_->Update();
        for (Actor::Enemy* enemy : enemies_) {
            enemy->Update();
        }
        const Rect viewRect = cameraController_.GetViewRect();
        player_->ConstrainToCamera(
            viewRect.left, viewRect.right, viewRect.bottom, viewRect.top);
        if (player_->IsDead()) {
            cameraController_.StopForcedScroll();
        }
        skyDome_->Update();
                                                                        
    }

    void GameScene::Draw() {
        KamataEngine::Model::PreDraw();

        for (auto box : currentMap_->GetBlocks()) {
            box->Draw(cameraController_.GetCamera());
        }
        
        player_->Draw(cameraController_.GetCamera());
        for (Actor::Enemy* enemy : enemies_) {
            enemy->Draw(cameraController_.GetCamera());
        }
        skyDome_->Draw(cameraController_.GetCamera());

        KamataEngine::Model::PostDraw();

       
    }
    void GameScene::Finalize() {
        for (Actor::Enemy* enemy : enemies_) {
            enemy->Finalize();
            delete enemy;
        }
        enemies_.clear();
        delete enemyModel_;
        enemyModel_ = nullptr;

        delete player_;
        delete skyDome_;

        currentMap_->Finalize();
        delete currentMap_;
        currentMap_ = nullptr;
    }
}
