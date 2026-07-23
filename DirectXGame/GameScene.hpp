#pragma once

#include <cstdint>
#include <memory>

#include "KamataEngine.h"
#include "CameraController.hpp"
#include "CharacterCollisionController.hpp"
#include "MapChipField.hpp"
#include "Player.hpp"
#include "SkyDome.hpp"

namespace Game {

class GameScene {
public:
    void Initialize();
    void Update();
    void Draw();
    void Finalize();

private:
    void GenerateMapData_();
    void GenerateMapObjects_();
    void GeneratePlayer_();

private:
    CameraController cameraController_{};
    Assets::CharacterCollisionController playerCollision_{};

    std::unique_ptr<SkyDome> skyDome_{};
    std::unique_ptr<Actor::Player> player_{};
    std::unique_ptr<Assets::MapChipField> currentMap_{};

    // GameScene 擁有共享資源；Box / MapObject / Player 都不 delete 它們。
    KamataEngine::Model* blockModel_ = nullptr;
    KamataEngine::Model* playerModel_ = nullptr;
    uint32_t blockTextureHandle_ = 0;

    uint32_t soundDataHandle_ = 0;
    uint32_t voiceHandle_ = 0;
};

} // namespace Game
