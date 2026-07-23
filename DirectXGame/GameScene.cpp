#include "GameScene.hpp"

#include "GameConfig.hpp"
#include "MapObjectFactory.hpp"

namespace Game {

void GameScene::Initialize() {
    cameraController_.Initialize();

    skyDome_ = std::make_unique<SkyDome>();
    skyDome_->Initialize(
        KamataEngine::Model::CreateFromOBJ("SkyDome", true),
        cameraController_.GetCamera());

    GenerateMapData_();
    GenerateMapObjects_();
    GeneratePlayer_();

    playerCollision_.Initialize({
        ::Player::Config::kPlayerWidth * 0.5f,
        ::Player::Config::kPlayerHeight * 0.5f,
        ::Player::Config::kPlayerDepth * 0.5f,
    });

    cameraController_.SetTarget(player_.get());
    cameraController_.SetMapField(currentMap_.get());
    cameraController_.ConstrainSideScrollCamera();
    cameraController_.GetCamera()->UpdateMatrix();
}

void GameScene::Update() {
    // 1. 地圖物件自己的 Component 更新。
    currentMap_->UpdateObjects();

    // 2. Player 只根據輸入計算本幀 velocity。
    player_->Update();

    // 3. 唯一修改物理位置的地方。
    playerCollision_.Move(*currentMap_, *player_);

    // 4. 所有位置修正完成後，只同步一次矩陣。
    player_->SyncTransform();

    // 5. 依賴 Player 最終位置的系統放在後面。
    cameraController_.Update();
    skyDome_->Update();
}

void GameScene::Draw() {
    KamataEngine::Model::PreDraw();

    currentMap_->DrawObjects(cameraController_.GetCamera());
    player_->Draw(cameraController_.GetCamera());
    skyDome_->Draw(cameraController_.GetCamera());

    KamataEngine::Model::PostDraw();
}

void GameScene::Finalize() {
    // Controller 保存 MapObject 的非擁有指標，必須先清掉。
    playerCollision_.ClearContacts();

    if (player_) {
        player_->Finalize();
        player_.reset();
    }

    if (currentMap_) {
        currentMap_->Finalize();
        currentMap_.reset();
    }

    skyDome_.reset();

    delete playerModel_;
    playerModel_ = nullptr;

    delete blockModel_;
    blockModel_ = nullptr;

    blockTextureHandle_ = 0;
}

void GameScene::GenerateMapData_() {
    currentMap_ = std::make_unique<Assets::MapChipField>();
    currentMap_->LoadData();
}

void GameScene::GenerateMapObjects_() {
    blockModel_ = KamataEngine::Model::Create();
    blockTextureHandle_ =
        KamataEngine::TextureManager::Load("images/Wall.png");

    Assets::MapObjectVisualDefinition sharedBoxVisual;
    sharedBoxVisual.model = blockModel_;
    sharedBoxVisual.textureHandle = blockTextureHandle_;
    sharedBoxVisual.scale = {1.0f, 1.0f, 1.0f};
    sharedBoxVisual.colliderHalfSize = {
        Block::Config::kBlockWidth * 0.5f,
        Block::Config::kBlockHeight * 0.5f,
        Block::Config::kBlockDepth * 0.5f,
    };

    Assets::MapObjectFactoryResources resources;
    resources.block = sharedBoxVisual;

    // 示例先共用外觀；日後只需要在這裡換各類型的 Model/Texture。
    resources.damageFloor = sharedBoxVisual;
    resources.breakableBox = sharedBoxVisual;
    resources.teleporter = sharedBoxVisual;
    resources.bounceFloor = sharedBoxVisual;

    resources.teleportDestination =
        currentMap_->GetPlayerRespawnPosition();

    Assets::MapObjectFactory factory(resources);
    currentMap_->BuildObjects(factory);
}

void GameScene::GeneratePlayer_() {
    playerModel_ = KamataEngine::Model::CreateFromOBJ("player");

    player_ = std::make_unique<Actor::Player>();
    player_->Initialize(
        playerModel_,
        currentMap_->GetPlayerRespawnPosition());
}

} // namespace Game
