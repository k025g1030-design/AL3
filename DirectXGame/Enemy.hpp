#pragma once

#include "KamataEngine.h"

namespace Actor {

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
    void Initialize(
        KamataEngine::Model* model,
        const KamataEngine::Vector3& position);
    void Update();
    void Draw(const KamataEngine::Camera* camera);
    void Finalize();

private:
    void ApplyTransform_();

private:
    KamataEngine::Model* model_ = nullptr;
    KamataEngine::WorldTransform worldTransform_;
    KamataEngine::Vector3 velocity_ = {};
    float walkTimer_ = 0.0f;

    // TODO: Enemy用モデルに合わせて、必要になったときだけ調整する値。
    static inline constexpr float kWalkSpeed = 0.05f;
    static inline constexpr float kWalkMotionAngleStart = -10.0f;
    static inline constexpr float kWalkMotionAngleEnd = 10.0f;
    static inline constexpr float kWalkMotionTime = 1.0f;
};

} // namespace Actor
