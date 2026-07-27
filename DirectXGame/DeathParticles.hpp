#pragma once

#include "KamataEngine.h"
#include <array>
#include <cstdint>
#include <numbers>

namespace Game {

/// <summary>
/// プレイヤー死亡時に8方向へ飛び散るモデルパーティクル
/// </summary>
class DeathParticles {
public:
    void Initialize(
        KamataEngine::Model* model, const KamataEngine::Camera* camera,
        const KamataEngine::Vector3& position);
    void Update();
    void Draw();

    bool IsFinished() const {
        return timer_ >= kDuration;
    }

private:
    static inline const uint32_t kNumParticles = 8;
    static inline const uint32_t kDuration = 60;
    static inline const float kSpeed = 0.2f;
    static inline const float kAngleUnit =
        2.0f * std::numbers::pi_v<float> /
        static_cast<float>(kNumParticles);

    KamataEngine::Model* model_ = nullptr;
    const KamataEngine::Camera* camera_ = nullptr;
    std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;
    uint32_t timer_ = 0;
};

} // namespace Game
