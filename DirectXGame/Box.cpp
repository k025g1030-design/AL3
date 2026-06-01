#include "Box.hpp"
#include "Math.hpp"

namespace Actor {
    void Box::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {
        assert(model);
        assert(camera);
        model_ = model;
        textureHandle_ = textureHandle;
        camera_ = camera;

        // ワールドトランスフォームの初期化
        worldTransform_.Initialize();
        worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
        worldTransform_.rotation_ = { 0.0f, 0.0f, 0.0f };
        worldTransform_.translation_ = { 0.0f, 0.0f, 50.0f };
    }
    void Box::Update() {
        worldTransform_.matWorld_ = MathUtils::MakeAffineMatrix(
            worldTransform_.scale_,
            worldTransform_.rotation_,
            worldTransform_.translation_
        );
        worldTransform_.TransferMatrix();
    }
    void Box::Draw() {

        model_->Draw(worldTransform_, *camera_, textureHandle_);

    }
    void Box::Draw(const KamataEngine::Camera& camera) {
        model_->Draw(worldTransform_, camera, textureHandle_);
    }
    void Box::Finalize() {
        KamataEngine::TextureManager::Unload(textureHandle_);
    }
}
