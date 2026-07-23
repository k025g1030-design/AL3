#pragma once

#include "MapCollisionTypes.hpp"

namespace Assets {

class IMapActor {
public:
    virtual ~IMapActor() = default;

    virtual void ApplyDamage(int damage) = 0;

    virtual KamataEngine::Vector3 GetPosition() const = 0;
    virtual void SetPosition(const KamataEngine::Vector3& position) = 0;

    virtual KamataEngine::Vector3 GetVelocity() const = 0;
    virtual void SetVelocity(const KamataEngine::Vector3& velocity) = 0;

    virtual void SetGrounded(bool grounded) = 0;
};

class IMapProjectile {
public:
    virtual ~IMapProjectile() = default;

    virtual int GetDamage() const = 0;
    virtual void Deactivate() = 0;
};

struct TouchEvent {
    IMapActor& actor;
    CollisionContact contact{};
    TouchPhase phase = TouchPhase::kStay;
};

struct ShootEvent {
    IMapProjectile& projectile;
    CollisionContact contact{};
};

struct ActiveEvent {
    IMapActor& actor;
};

} // namespace Assets
