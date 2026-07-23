#pragma once

#include <unordered_set>

#include "MapChipField.hpp"
#include "MapObjectEvents.hpp"

namespace Assets {

    // 一个实例对应一个需要追踪Enter/Stay/Exit的Actor（通常就是Player）。
    class MapCollisionDispatcher {
    public:
        void ProcessActor(
            MapChipField& field,
            IMapActor& actor,
            MapAABB& actorCollider);

        bool ProcessProjectile(
            MapChipField& field,
            IMapProjectile& projectile,
            const MapAABB& projectileCollider);

        void Activate(MapObject& object, IMapActor& actor);

        // 地图重载、场景切换前调用，避免保留旧MapObject指针。
        void ClearContacts();

    private:
        void ResolveSolidContact_(
            IMapActor& actor,
            MapAABB& actorCollider,
            const CollisionContact& contact);

    private:
        std::unordered_set<MapObject*> previousContacts_{};
    };

} // namespace Assets
