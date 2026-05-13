#include "Player.hpp"

namespace Actor {
    void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {
        assert(model);
        assert(camera);
        model_ = model;
        textureHandle_ = textureHandle;
        camera_ = camera;

        // ワールドトランスフォームの初期化
        worldTransform_.Initialize();
        worldTransform_.scale_ = { 0.1f, 0.1f, 0.1f };
        worldTransform_.rotation_ = { 0.0f, 0.0f, 0.0f };
        worldTransform_.translation_ = { 0.0f, 0.0f, 50.0f };
    }
    void Player::Update() {
        worldTransform_.TransferMatrix();
    }
    void Player::Draw() {

        model_->Draw(worldTransform_, *camera_, textureHandle_);

    }
    void Player::Finalize() {
        KamataEngine::TextureManager::Unload(textureHandle_);
    }
}
