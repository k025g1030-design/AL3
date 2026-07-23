#pragma once

#include <numbers>

#include "GameConfig.hpp"
#include "KamataEngine.h"
#include "MapObjectEvents.hpp"
#include "Math.hpp"

namespace Actor {

enum class LRDirection {
    kRight = 0,
    kLeft = 1,
};

class Player final : public Assets::IMapActor {
public:
    void Initialize(
        KamataEngine::Model* model,
        const KamataEngine::Vector3& position);

    // 只計算輸入、速度與朝向，不在這裡修改位置。
    void Update();

    // 碰撞控制器完成整幀的位置修正後，只同步一次描畫矩陣。
    void SyncTransform();

    void Draw(const KamataEngine::Camera* camera);
    void Finalize();

    // IMapActor
    void ApplyDamage(int damage) override;
    KamataEngine::Vector3 GetPosition() const override;
    void SetPosition(const KamataEngine::Vector3& position) override;
    KamataEngine::Vector3 GetVelocity() const override;
    void SetVelocity(const KamataEngine::Vector3& velocity) override;
    void SetGrounded(bool grounded) override;

    KamataEngine::WorldTransform& GetWorldTransform();
    const KamataEngine::WorldTransform& GetWorldTransform() const;

    bool IsGrounded() const;
    int GetHealth() const;

private:
    void StartTurn_();
    void UpdateTurn_();
    float EaseOutBounce_(float t) const;
    float EaseInOutBounce_(float t) const;
    void ApplyTransform_();

private:
    // GameScene / ResourceManager 擁有 Model。
    KamataEngine::Model* model_ = nullptr;
    KamataEngine::WorldTransform worldTransform_{};

    KamataEngine::Vector3 velocity_{0.0f, 0.0f, 0.0f};
    LRDirection lrDirection_ = LRDirection::kRight;

    float turnFirstRotationY_ = 0.0f;
    float turnTimer_ = 0.0f;

    bool onGround_ = false;

    static constexpr int kMaxHealth_ = 100;
    int health_ = kMaxHealth_;
};

} // namespace Actor
