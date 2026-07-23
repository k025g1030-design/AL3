#include "MapObjectComponents.hpp"

#include <algorithm>

#include "MapObject.hpp"

namespace Assets {

DamageOnTouchComponent::DamageOnTouchComponent(
    int damage,
    TouchPhase triggerPhase)
    : damage_(damage), triggerPhase_(triggerPhase) {}

void DamageOnTouchComponent::OnTouch(
    MapObject&,
    const TouchEvent& event) {

    if (event.phase == triggerPhase_) {
        event.actor.ApplyDamage(damage_);
    }
}

TeleportOnTouchComponent::TeleportOnTouchComponent(
    const KamataEngine::Vector3& destination)
    : destination_(destination) {}

void TeleportOnTouchComponent::OnTouch(
    MapObject&,
    const TouchEvent& event) {

    if (event.phase == TouchPhase::kEnter) {
        event.actor.SetPosition(destination_);
    }
}

BounceOnTouchComponent::BounceOnTouchComponent(float verticalSpeed)
    : verticalSpeed_(verticalSpeed) {}

void BounceOnTouchComponent::OnTouch(
    MapObject&,
    const TouchEvent& event) {

    if (event.phase != TouchPhase::kEnter || event.contact.normal.y <= 0.7f) {
        return;
    }

    KamataEngine::Vector3 velocity = event.actor.GetVelocity();
    velocity.y = verticalSpeed_;
    event.actor.SetVelocity(velocity);
}

BreakableOnShootComponent::BreakableOnShootComponent(int health)
    : health_(max(1, health)) {}

void BreakableOnShootComponent::OnShoot(
    MapObject& owner,
    const ShootEvent& event) {

    health_ -= event.projectile.GetDamage();
    event.projectile.Deactivate();

    if (health_ <= 0) {
        owner.SetActive(false);
    }
}

ToggleTargetOnActiveComponent::ToggleTargetOnActiveComponent(MapObject* target)
    : target_(target) {}

void ToggleTargetOnActiveComponent::OnActive(
    MapObject&,
    const ActiveEvent&) {

    if (target_ != nullptr) {
        target_->SetActive(!target_->IsActive());
    }
}

} // namespace Assets
