#pragma once

#include "KamataEngine.h"

namespace Collision {

struct AABB {
    KamataEngine::Vector3 min;
    KamataEngine::Vector3 max;
};

inline bool IsCollision(const AABB& a, const AABB& b) {
    return a.min.x <= b.max.x && a.max.x >= b.min.x &&
           a.min.y <= b.max.y && a.max.y >= b.min.y &&
           a.min.z <= b.max.z && a.max.z >= b.min.z;
}

} // namespace Collision
