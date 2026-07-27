#pragma once

#include "KamataEngine.h"
#include "Collision.hpp"

namespace Actor {

class Player;

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
    KamataEngine::Vector3 GetWorldPosition() const;
    Collision::AABB GetAABB() const;
    void OnCollision(const Player* player);

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
    static inline constexpr float kWidth = 0.8f;
    static inline constexpr float kHeight = 0.8f;
    static inline constexpr float kDepth = 0.8f;
};

} // namespace Actor
