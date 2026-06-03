#pragma once
#include "KamataEngine.h"
#include <cstdint>	
#include "SkyDome.hpp"

#include "Player.hpp"
#include "Box.hpp"

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

	private:
        uint32_t soundDataHandle_ = 0;
        uint32_t voiceHandle_ = 0;
        SkyDome* skyDome_ = nullptr;

        bool debugCameraActive_ = true;
        
        

        KamataEngine::DebugCamera* debugCamera_ = nullptr;
        KamataEngine::Camera camera_;

        std::vector<std::vector<int>> stageData_ = {
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
        };  

        Actor::Player* player_ = nullptr;
        std::vector<Actor::Box*> boxes_;


        
	};

}