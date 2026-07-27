#include "DeathParticles.hpp"
#include "Math.hpp"
#include <cassert>

namespace Game {

void DeathParticles::Initialize(
    KamataEngine::Model* model, const KamataEngine::Camera* camera,
    const KamataEngine::Vector3& position) {
    assert(model);
    assert(camera);
    model_ = model;
    camera_ = camera;
    timer_ = 0;

    for (KamataEngine::WorldTransform& worldTransform : worldTransforms_) {
        worldTransform.Initialize();
        worldTransform.scale_ = {0.5f, 0.5f, 0.5f};
        worldTransform.translation_ = position;
    }
}

void DeathParticles::Update() {
    if (IsFinished()) {
        return;
    }

    for (uint32_t i = 0; i < kNumParticles; ++i) {
        KamataEngine::Vector3 velocity = {kSpeed, 0.0f, 0.0f};
        const float angle = kAngleUnit * static_cast<float>(i);
        const KamataEngine::Matrix4x4 rotation =
            MathUtils::MakeRotateZMatrix(angle);
        velocity = MathUtils::Transform(velocity, rotation);

        KamataEngine::WorldTransform& worldTransform = worldTransforms_[i];
        worldTransform.translation_ =
            MathUtils::V3Plus(worldTransform.translation_, velocity);
        worldTransform.matWorld_ = MathUtils::MakeAffineMatrix(
            worldTransform.scale_, worldTransform.rotation_,
            worldTransform.translation_);
        worldTransform.TransferMatrix();
    }

    ++timer_;
}

void DeathParticles::Draw() {
    if (IsFinished() || model_ == nullptr || camera_ == nullptr) {
        return;
    }

    for (const KamataEngine::WorldTransform& worldTransform :
         worldTransforms_) {
        model_->Draw(worldTransform, *camera_);
    }
}

} // namespace Game
