#pragma once
#include "KamataEngine.h"
#include <cstdint>	
#include "SkyDome.hpp"
#include "MapChipField.hpp"
#include "CameraController.hpp"
#include "DeathParticles.hpp"

#include "Player.hpp"
#include "Enemy.hpp"
#include <vector>


namespace Game{
	class GameScene {
	public:
        void Initialize();
        void Update();
        void Draw();
        void Finalize();
        void CheckAllCollisions();

    public:
        const uint32_t modelsize = 512;
        

        void GenerateBlocks() {
            // stageData_をもとにBoxを生成
            for (uint32_t y = 0; y < currentMap_->GetNumBlockVertical(); y++) {
                for (uint32_t x = 0; x < currentMap_->GetNumBlockHorizontal(); x++) {
                    if (currentMap_->GetMapChipTypeByIndex(x, y) == Assets::MapChipType::kBlock) {
                        Assets::Box* box = new Assets::Box();
                        box->Initialize(
                            KamataEngine::Model::Create(), 
                            KamataEngine::TextureManager::Load("images/Wall.png"), 
                            currentMap_->GetMapChipPositionByIndex(x, y)
                        );
                        currentMap_->AddBlock(box);
                    }
                }
            }
        }

        void GeneratePlayer() {
            player_ = new Actor::Player();
            player_->Initialize(KamataEngine::Model::CreateFromOBJ("player"), currentMap_->GetPlayerRespawnPosition());
        }

        void GenerateEnemies() {
            enemyModel_ = KamataEngine::Model::CreateFromOBJ("enemy");
            for (const KamataEngine::Vector3& position :
                 currentMap_->GetEnemyRespawnPositions()) {
                Actor::Enemy* enemy = new Actor::Enemy();
                enemy->Initialize(enemyModel_, position);
                enemies_.push_back(enemy);
            }
        }

        void GenerateMapData() {
            currentMap_ = new Assets::MapChipField();
            currentMap_->LoadData();
        }

	private:
        uint32_t soundDataHandle_ = 0;
        uint32_t voiceHandle_ = 0;
        SkyDome* skyDome_ = nullptr;

        

        Game::CameraController cameraController_;



        Actor::Player* player_ = nullptr;
        KamataEngine::Model* enemyModel_ = nullptr;
        std::vector<Actor::Enemy*> enemies_;

        Assets::MapChipField* currentMap_ = nullptr;
        KamataEngine::Model* deathParticleModel_ = nullptr;
        DeathParticles* deathParticles_ = nullptr;
        bool wasPlayerDead_ = false;
       


        
	};

}
