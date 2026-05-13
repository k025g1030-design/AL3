#pragma once
#include "KamataEngine.h"
#include <cstdint>	

namespace Game{
	class GameScene {
	public:
        void Initialize();
        void Update();
        void Draw();
        void Finalize();

	private:
        uint32_t textureHandle_ = 0;
        uint32_t soundDataHandle_ = 0;
        uint32_t voiceHandle_ = 0;

        KamataEngine::Model* model_ = nullptr;
        KamataEngine::DebugCamera* debugCamera_ = nullptr;
        KamataEngine::WorldTransform worldTransform_;
        KamataEngine::Camera camera_;

	};

}