#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "Box.hpp"
#include "MapCollisionTypes.hpp"
#include "IMapObjectComponent.hpp"

namespace Assets {

class MapObject {
public:
    MapObject() = default;
    ~MapObject() = default;

    MapObject(const MapObject&) = delete;
    MapObject& operator=(const MapObject&) = delete;

    void InitializeVisual(
        KamataEngine::Model* model,
        uint32_t textureHandle,
        const KamataEngine::Vector3& position);

    void Finalize();
    void Update();
    void Draw(const KamataEngine::Camera* camera);

    template<class T, class... Args>
    T& AddComponent(Args&&... args) {
        auto component =
            std::make_unique<T>(std::forward<Args>(args)...);

        T& result = *component;
        components_.push_back(std::move(component));
        return result;
    }

    void DispatchTouch(const TouchEvent& event);
    void DispatchShoot(const ShootEvent& event);
    void DispatchActive(const ActiveEvent& event);

    void SetPosition(const KamataEngine::Vector3& position);
    const KamataEngine::Vector3& GetPosition() const;

    void SetScale(const KamataEngine::Vector3& scale);

    void SetCollider(
        const KamataEngine::Vector3& halfSize,
        const KamataEngine::Vector3& centerOffset = {0.0f, 0.0f, 0.0f});

    const MapAABB& GetCollider() const;
    bool HasCollider() const;

    void SetSolid(bool solid);
    bool IsSolid() const;

    void SetTrigger(bool trigger);
    bool IsTrigger() const;

    void SetActive(bool active);
    bool IsActive() const;

private:
    void RefreshColliderPosition_();

private:
    KamataEngine::Vector3 position_{0.0f, 0.0f, 0.0f};

    Box visual_{};
    bool hasVisual_ = false;

    MapAABB collider_{};
    KamataEngine::Vector3 colliderCenterOffset_{0.0f, 0.0f, 0.0f};
    bool hasCollider_ = false;

    bool isSolid_ = false;
    bool isTrigger_ = false;
    bool isActive_ = true;

    std::vector<std::unique_ptr<IMapObjectComponent>> components_{};
};

} // namespace Assets
