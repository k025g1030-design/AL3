#include "Box.hpp"


namespace Assets {
    void Box::Initialize(KamataEngine::Model* model, uint32_t textureHandle, const KamataEngine::Vector3& position) {
        assert(model);
        model_ = model;
        textureHandle_ = textureHandle;
        

        // ワールドトランスフォームの初期化
        worldTransform_.Initialize();
        worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
        worldTransform_.rotation_ = { 0.0f, 0.0f, 0.0f };
        worldTransform_.translation_ = position;
    }
    void Box::Update() {
        ApplyTransform_();
    }
    
    void Box::Draw(const KamataEngine::Camera* camera) {
        model_->Draw(worldTransform_, *camera, textureHandle_);
    }
    void Box::Finalize() {
        KamataEngine::TextureManager::Unload(textureHandle_);
        delete model_;
        model_ = nullptr;
        
    }
}
