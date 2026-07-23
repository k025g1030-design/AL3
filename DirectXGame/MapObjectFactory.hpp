#pragma once

#include <memory>

#include "MapChipField.hpp"
#include "MapObjectComponents.hpp"

namespace Assets {

struct MapObjectVisualDefinition {
    KamataEngine::Model* model = nullptr; // ResourceManager owns
    uint32_t textureHandle = 0;

    KamataEngine::Vector3 scale{1.0f, 1.0f, 1.0f};
    KamataEngine::Vector3 colliderHalfSize{0.5f, 0.5f, 0.5f};
    KamataEngine::Vector3 colliderCenterOffset{0.0f, 0.0f, 0.0f};
};

struct MapObjectFactoryResources {
    MapObjectVisualDefinition block{};
    MapObjectVisualDefinition damageFloor{};
    MapObjectVisualDefinition breakableBox{};
    MapObjectVisualDefinition teleporter{};
    MapObjectVisualDefinition bounceFloor{};

    KamataEngine::Vector3 teleportDestination{0.0f, 0.0f, 0.0f};
};

class MapObjectFactory {
public:
    explicit MapObjectFactory(const MapObjectFactoryResources& resources);

    std::unique_ptr<MapObject> Create(
        MapChipType type,
        const KamataEngine::Vector3& position) const;

private:
    std::unique_ptr<MapObject> CreateBase_(
        const MapObjectVisualDefinition& visual,
        const KamataEngine::Vector3& position,
        bool solid,
        bool trigger) const;

private:
    MapObjectFactoryResources resources_{};
};

} // namespace Assets
