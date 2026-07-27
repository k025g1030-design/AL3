#include "Enemy.hpp"

#include "Math.hpp"

#include <cassert>
#include <cmath>
#include <numbers>

namespace Actor {

void Enemy::Initialize(
    KamataEngine::Model* model,
    const KamataEngine::Vector3& position) {
    assert(model);
    model_ = model;

    worldTransform_.Initialize();
    worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
    worldTransform_.rotation_ = {
        0.0f, std::numbers::pi_v<float> * 3.0f / 2.0f, 0.0f};
    worldTransform_.translation_ = position;

    velocity_ = {-kWalkSpeed, 0.0f, 0.0f};
    walkTimer_ = 0.0f;
    ApplyTransform_();
}

void Enemy::Update() {
    constexpr float kDeltaTime = 1.0f / 60.0f;
    walkTimer_ += kDeltaTime;

    worldTransform_.translation_.x += velocity_.x;
    worldTransform_.translation_.y += velocity_.y;
    worldTransform_.translation_.z += velocity_.z;

    const float parameter = std::sin(
        2.0f * std::numbers::pi_v<float> * walkTimer_ / kWalkMotionTime);
    const float degree =
        kWalkMotionAngleStart +
        (kWalkMotionAngleEnd - kWalkMotionAngleStart) *
            (parameter + 1.0f) / 2.0f;
    worldTransform_.rotation_.x =
        degree * std::numbers::pi_v<float> / 180.0f;

    ApplyTransform_();
}

void Enemy::Draw(const KamataEngine::Camera* camera) {
    assert(camera);
    model_->Draw(worldTransform_, *camera);
}

void Enemy::Finalize() {
    model_ = nullptr;
}

void Enemy::ApplyTransform_() {
    worldTransform_.matWorld_ = MathUtils::MakeAffineMatrix(
        worldTransform_.scale_,
        worldTransform_.rotation_,
        worldTransform_.translation_);
    worldTransform_.TransferMatrix();
}

} // namespace Actor
