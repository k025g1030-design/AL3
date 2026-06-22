#pragma once
#include "KamataEngine.h"
#include <cstdint>	
#include "SkyDome.hpp"
#include "MapChipField.hpp"

#include "Player.hpp"


namespace Game{
	class GameScene {
	public:
        void Initialize();
        void Update();
        void Draw();
        void Finalize();

    public:
        const uint32_t modelsize = 512;
        void SetDebugCameraActive(bool active) {
            debugCameraActive_ = active;
        }

        void GenerateBlocks() {
            // stageData_をもとにBoxを生成
            for (uint32_t y = 0; y < mapChipField_.GetNumBlockVertical(); y++) {
                for (uint32_t x = 0; x < mapChipField_.GetNumBlockHorizontal(); x++) {
                    if (mapChipField_.GetMapChipTypeByIndex(x, y) == Assets::MapChipType::kBlock) {
                        Assets::Box* box = new Assets::Box();
                        box->Initialize(
                            KamataEngine::Model::Create(), 
                            KamataEngine::TextureManager::Load("images/Wall.png"), 
                            &camera_, 
                            mapChipField_.GetMapChipPositionByIndex(x, y)
                        );
                        mapChipField_.AddBlock(box);
                    }
                }
            }
        }

        void GeneratePlayer() {
            player_ = new Actor::Player();
            player_->Initialize(KamataEngine::Model::CreateFromOBJ("player"), &camera_, mapChipField_.GetPlayerRespawnPosition());
        }

	private:
        uint32_t soundDataHandle_ = 0;
        uint32_t voiceHandle_ = 0;
        SkyDome* skyDome_ = nullptr;

        bool debugCameraActive_ = false;

        KamataEngine::DebugCamera* debugCamera_ = nullptr;
        KamataEngine::Camera camera_;



        Actor::Player* player_ = nullptr;

        Assets::MapChipField mapChipField_;
       
       


        
	};

}