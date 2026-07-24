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
        

        cameraController_.SetTarget(player_);
        cameraController_.SetMapField(currentMap_);
        cameraController_.ConstrainSideScrollCamera();
        cameraController_.GetCamera()->UpdateMatrix();

        
    }

    void GameScene::Update() {

        cameraController_.Update();
        

        for (auto box : currentMap_->GetBlocks()) {
            box->Update();
        }
        player_->Update();
        skyDome_->Update();
                                                                        
    }

    void GameScene::Draw() {
        KamataEngine::Model::PreDraw();

        for (auto box : currentMap_->GetBlocks()) {
            box->Draw(cameraController_.GetCamera());
        }
        
        player_->Draw(cameraController_.GetCamera());
        skyDome_->Draw(cameraController_.GetCamera());

        KamataEngine::Model::PostDraw();

       
    }
    void GameScene::Finalize() {
        delete player_;
        delete skyDome_;

        currentMap_->Finalize();
    }
}
