#include "Box.hpp"

namespace Assets {

void Box::Initialize(
    KamataEngine::Model* model,
    uint32_t textureHandle,
    const KamataEngine::Vector3& position) {

    assert(model != nullptr);

    model_ = model;
    textureHandle_ = textureHandle;

    worldTransform_.Initialize();
    worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
    worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
    worldTransform_.translation_ = position;
    ApplyTransform_();
}

void Box::Update() {
    ApplyTransform_();
}

void Box::Draw(const KamataEngine::Camera* camera) {
    if (model_ == nullptr || camera == nullptr) {
        return;
    }

    model_->Draw(worldTransform_, *camera, textureHandle_);
}

void Box::Finalize() {
    // Model和Texture通常被所有地图方块共享，所以不在这里delete/Unload。
    model_ = nullptr;
    textureHandle_ = 0;
}

void Box::SetPosition(const KamataEngine::Vector3& position) {
    worldTransform_.translation_ = position;
}

const KamataEngine::Vector3& Box::GetPosition() const {
    return worldTransform_.translation_;
}

void Box::SetScale(const KamataEngine::Vector3& scale) {
    worldTransform_.scale_ = scale;
}

const KamataEngine::Vector3& Box::GetScale() const {
    return worldTransform_.scale_;
}

void Box::SetRotation(const KamataEngine::Vector3& rotation) {
    worldTransform_.rotation_ = rotation;
}

const KamataEngine::Vector3& Box::GetRotation() const {
    return worldTransform_.rotation_;
}

void Box::ApplyTransform_() {
    worldTransform_.matWorld_ = MathUtils::MakeAffineMatrix(
        worldTransform_.scale_,
        worldTransform_.rotation_,
        worldTransform_.translation_);

    worldTransform_.TransferMatrix();
}

} // namespace Assets
