#pragma once
#include "KamataEngine.h"
#include <cstdint>	

#include "Player.hpp"

namespace Game{
	class GameScene {
	public:
        void Initialize();
        void Update();
        void Draw();
        void Finalize();

	private:
        uint32_t soundDataHandle_ = 0;
        uint32_t voiceHandle_ = 0;

        KamataEngine::DebugCamera* debugCamera_ = nullptr;
        KamataEngine::Camera camera_;

        Actor::Player* player_ = nullptr;


	};

}