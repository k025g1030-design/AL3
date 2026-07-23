#include "MapObject.hpp"

namespace Assets {

void MapObject::InitializeVisual(
    KamataEngine::Model* model,
    uint32_t textureHandle,
    const KamataEngine::Vector3& position) {

    position_ = position;
    visual_.Initialize(model, textureHandle, position_);
    hasVisual_ = true;
    RefreshColliderPosition_();
}

void MapObject::Finalize() {
    if (hasVisual_) {
        visual_.Finalize();
    }

    components_.clear();
    hasVisual_ = false;
    hasCollider_ = false;
}

void MapObject::Update() {
    if (!isActive_) {
        return;
    }

    for (auto& component : components_) {
        component->Update(*this);
    }

    if (hasVisual_) {
        visual_.Update();
    }
}

void MapObject::Draw(const KamataEngine::Camera* camera) {
    if (!isActive_ || !hasVisual_) {
        return;
    }

    visual_.Draw(camera);
}

void MapObject::DispatchTouch(const TouchEvent& event) {
    if (!isActive_) {
        return;
    }

    for (auto& component : components_) {
        component->OnTouch(*this, event);
    }
}

void MapObject::DispatchShoot(const ShootEvent& event) {
    if (!isActive_) {
        return;
    }

    for (auto& component : components_) {
        component->OnShoot(*this, event);
    }
}

void MapObject::DispatchActive(const ActiveEvent& event) {
    if (!isActive_) {
        return;
    }

    for (auto& component : components_) {
        component->OnActive(*this, event);
    }
}

void MapObject::SetPosition(const KamataEngine::Vector3& position) {
    position_ = position;

    if (hasVisual_) {
        visual_.SetPosition(position_);
    }

    RefreshColliderPosition_();
}

const KamataEngine::Vector3& MapObject::GetPosition() const {
    return position_;
}

void MapObject::SetScale(const KamataEngine::Vector3& scale) {
    if (hasVisual_) {
        visual_.SetScale(scale);
    }
}

void MapObject::SetCollider(
    const KamataEngine::Vector3& halfSize,
    const KamataEngine::Vector3& centerOffset) {

    collider_.halfSize = halfSize;
    colliderCenterOffset_ = centerOffset;
    hasCollider_ = true;
    RefreshColliderPosition_();
}

const MapAABB& MapObject::GetCollider() const {
    return collider_;
}

bool MapObject::HasCollider() const {
    return hasCollider_;
}

void MapObject::SetSolid(bool solid) {
    isSolid_ = solid;
}

bool MapObject::IsSolid() const {
    return isSolid_;
}

void MapObject::SetTrigger(bool trigger) {
    isTrigger_ = trigger;
}

bool MapObject::IsTrigger() const {
    return isTrigger_;
}

void MapObject::SetActive(bool active) {
    isActive_ = active;
}

bool MapObject::IsActive() const {
    return isActive_;
}

void MapObject::RefreshColliderPosition_() {
    collider_.center = {
        position_.x + colliderCenterOffset_.x,
        position_.y + colliderCenterOffset_.y,
        position_.z + colliderCenterOffset_.z,
    };
}

} // namespace Assets
