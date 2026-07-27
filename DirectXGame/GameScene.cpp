#include "GameScene.hpp"
#include "KamataEngine.h"


namespace Game {
    void GameScene::Initialize() {
        phase_ = Phase::kPlay;
        finished_ = false;
        cameraController_.Initialize();

        // skyDomeの初期化
        skyDome_ = new Game::SkyDome();
        skyDomeModel_ = KamataEngine::Model::CreateFromOBJ("SkyDome", true);
        skyDome_->Initialize(skyDomeModel_, cameraController_.GetCamera());

        GenerateMapData();
        GenerateBlocks();

        // player initialization should be after boxes initialization to ensure the player is drawn on top of the boxes

        GeneratePlayer();
        player_->SetMapChipField(currentMap_);
        GenerateEnemies();
        // 専用モデルを用意するまではプレイヤーモデルで代用する。
        deathParticleModel_ =
            KamataEngine::Model::CreateFromOBJ("player");

        cameraController_.SetTarget(player_);
        cameraController_.SetMapField(currentMap_);
        cameraController_.ConstrainSideScrollCamera();
        cameraController_.GetCamera()->UpdateMatrix();

        
    }

    void GameScene::Update() {
        switch (phase_) {
        case Phase::kPlay:
            UpdatePlayPhase();
            break;
        case Phase::kDeath:
            UpdateDeathPhase();
            break;
        }
        ChangePhase();
    }

    void GameScene::UpdatePlayPhase() {
        KamataEngine::Input* input = KamataEngine::Input::GetInstance();
        if (input->TriggerKey(DIK_P)) {
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
        CheckAllCollisions();
        const Rect viewRect = cameraController_.GetViewRect();
        player_->ConstrainToCamera(
            viewRect.left, viewRect.right, viewRect.bottom, viewRect.top);

        skyDome_->Update();
    }

    void GameScene::UpdateDeathPhase() {
        for (auto box : currentMap_->GetBlocks()) {
            box->Update();
        }
        for (Actor::Enemy* enemy : enemies_) {
            enemy->Update();
        }
        if (deathParticles_ != nullptr) {
            deathParticles_->Update();
            if (deathParticles_->IsFinished()) {
                finished_ = true;
            }
        }
        skyDome_->Update();
    }

    void GameScene::ChangePhase() {
        switch (phase_) {
        case Phase::kPlay:
            if (player_->IsDead()) {
                phase_ = Phase::kDeath;
                cameraController_.StopForcedScroll();
                deathParticles_ = new DeathParticles();
                deathParticles_->Initialize(
                    deathParticleModel_, cameraController_.GetCamera(),
                    player_->GetWorldPosition());
            }
            break;
        case Phase::kDeath:
            break;
        }
    }

    void GameScene::CheckAllCollisions() {
        const Collision::AABB playerAABB = player_->GetAABB();
        for (Actor::Enemy* enemy : enemies_) {
            if (Collision::IsCollision(playerAABB, enemy->GetAABB())) {
                player_->OnCollision(enemy);
                enemy->OnCollision(player_);
            }
        }
    }

    void GameScene::Draw() {
        KamataEngine::Model::PreDraw();

        for (auto box : currentMap_->GetBlocks()) {
            box->Draw(cameraController_.GetCamera());
        }
        
        if (phase_ == Phase::kPlay) {
            player_->Draw(cameraController_.GetCamera());
        }
        for (Actor::Enemy* enemy : enemies_) {
            enemy->Draw(cameraController_.GetCamera());
        }
        if (deathParticles_ != nullptr) {
            deathParticles_->Draw();
        }
        skyDome_->Draw(cameraController_.GetCamera());

        KamataEngine::Model::PostDraw();

       
    }
    void GameScene::Finalize() {
        delete deathParticles_;
        deathParticles_ = nullptr;
        delete deathParticleModel_;
        deathParticleModel_ = nullptr;

        for (Actor::Enemy* enemy : enemies_) {
            enemy->Finalize();
            delete enemy;
        }
        enemies_.clear();
        delete enemyModel_;
        enemyModel_ = nullptr;

        if (player_ != nullptr) {
            player_->Finalize();
        }
        delete player_;
        player_ = nullptr;
        delete playerModel_;
        playerModel_ = nullptr;

        if (skyDome_ != nullptr) {
            skyDome_->Finalize();
        }
        delete skyDome_;
        skyDome_ = nullptr;
        delete skyDomeModel_;
        skyDomeModel_ = nullptr;

        currentMap_->Finalize();
        delete currentMap_;
        currentMap_ = nullptr;
    }
}
