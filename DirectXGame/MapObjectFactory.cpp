#include "MapObjectFactory.hpp"

namespace Assets {

MapObjectFactory::MapObjectFactory(
    const MapObjectFactoryResources& resources)
    : resources_(resources) {}

std::unique_ptr<MapObject> MapObjectFactory::Create(
    MapChipType type,
    const KamataEngine::Vector3& position) const {

    std::unique_ptr<MapObject> object;

    switch (type) {
    case MapChipType::kBlock:
        object = CreateBase_(resources_.block, position, true, false);
        break;

    case MapChipType::kDamageFloor:
        object = CreateBase_(resources_.damageFloor, position, true, false);
        object->AddComponent<DamageOnTouchComponent>(
            10,
            TouchPhase::kEnter);
        break;

    case MapChipType::kBreakableBox:
        object = CreateBase_(resources_.breakableBox, position, true, false);
        object->AddComponent<BreakableOnShootComponent>(3);
        break;

    case MapChipType::kTeleporter:
        object = CreateBase_(resources_.teleporter, position, false, true);
        object->AddComponent<TeleportOnTouchComponent>(
            resources_.teleportDestination);
        break;

    case MapChipType::kBounceFloor:
        object = CreateBase_(resources_.bounceFloor, position, true, false);
        object->AddComponent<BounceOnTouchComponent>(
            World::Config::kBounceSpeed);
        break;

    // Blank和出生点只是地图数据，不生成运行时MapObject。
    case MapChipType::kBlank:
    case MapChipType::kPlayerRespawn:
    case MapChipType::kEnemyRespawn:
    default:
        return nullptr;
    }

    return object;
}

std::unique_ptr<MapObject> MapObjectFactory::CreateBase_(
    const MapObjectVisualDefinition& visual,
    const KamataEngine::Vector3& position,
    bool solid,
    bool trigger) const {

    auto object = std::make_unique<MapObject>();
    object->SetPosition(position);

    if (visual.model != nullptr) {
        object->InitializeVisual(
            visual.model,
            visual.textureHandle,
            position);
        object->SetScale(visual.scale);
    }

    object->SetCollider(
        visual.colliderHalfSize,
        visual.colliderCenterOffset);

    object->SetSolid(solid);
    object->SetTrigger(trigger);
    return object;
}

} // namespace Assets
