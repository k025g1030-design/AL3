#pragma once

#include "IMapObjectComponent.hpp"

namespace Assets {

class DamageOnTouchComponent final : public IMapObjectComponent {
public:
    explicit DamageOnTouchComponent(
        int damage,
        TouchPhase triggerPhase = TouchPhase::kEnter);

    void OnTouch(MapObject& owner, const TouchEvent& event) override;

private:
    int damage_ = 0;
    TouchPhase triggerPhase_ = TouchPhase::kEnter;
};

class TeleportOnTouchComponent final : public IMapObjectComponent {
public:
    explicit TeleportOnTouchComponent(
        const KamataEngine::Vector3& destination);

    void OnTouch(MapObject& owner, const TouchEvent& event) override;

private:
    KamataEngine::Vector3 destination_{0.0f, 0.0f, 0.0f};
};

class BounceOnTouchComponent final : public IMapObjectComponent {
public:
    explicit BounceOnTouchComponent(float verticalSpeed);

    void OnTouch(MapObject& owner, const TouchEvent& event) override;

private:
    float verticalSpeed_ = 0.0f;
};

class BreakableOnShootComponent final : public IMapObjectComponent {
public:
    explicit BreakableOnShootComponent(int health);

    void OnShoot(MapObject& owner, const ShootEvent& event) override;

private:
    int health_ = 1;
};

class ToggleTargetOnActiveComponent final : public IMapObjectComponent {
public:
    explicit ToggleTargetOnActiveComponent(MapObject* target);

    void OnActive(MapObject& owner, const ActiveEvent& event) override;

private:
    MapObject* target_ = nullptr; // non-owning
};

} // namespace Assets
