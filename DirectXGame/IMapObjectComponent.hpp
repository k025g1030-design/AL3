#pragma once

#include "MapObjectEvents.hpp"

namespace Assets {

    class MapObject;

    class IMapObjectComponent {
    public:
        virtual ~IMapObjectComponent() = default;

        virtual void Update(MapObject&) {}
        virtual void OnTouch(MapObject&, const TouchEvent&) {}
        virtual void OnShoot(MapObject&, const ShootEvent&) {}
        virtual void OnActive(MapObject&, const ActiveEvent&) {}
    };

} // namespace Assets
