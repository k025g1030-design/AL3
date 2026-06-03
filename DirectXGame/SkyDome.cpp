#include "SkyDome.hpp"


namespace Game {
    void SkyDome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
        assert(model);
        assert(camera);
        model_ = model;
        camera_ = camera;

        // ワールドトランスフォームの初期化
        worldTransform_.Initialize();
        worldTransform_.scale_ = { 0.05f, 0.05f, 0.05f };
        worldTransform_.rotation_ = { 0, 0, 0 };
        worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
    }
    void SkyDome::Update() {
        worldTransform_.matWorld_ = MathUtils::MakeAffineMatrix(
            worldTransform_.scale_,
            worldTransform_.rotation_,
            worldTransform_.translation_
        );
        worldTransform_.TransferMatrix();
    }
    void SkyDome::Draw() {

        model_->Draw(worldTransform_, *camera_);

    }
    void SkyDome::Draw(const KamataEngine::Camera& camera) {
        model_->Draw(worldTransform_, camera);
    }
    void SkyDome::Finalize() {
        model_ = nullptr;
    }

}