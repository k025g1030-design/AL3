#pragma once

#include <algorithm>
#include <cmath>

#include "GameConfig.hpp"
#include "KamataEngine.h"

namespace Assets {

struct MapAABB {
    KamataEngine::Vector3 center{0.0f, 0.0f, 0.0f};
    KamataEngine::Vector3 halfSize{0.5f, 0.5f, 0.5f};
};

struct CollisionContact {
    // normal 指向「把 moving 推出 fixed」的方向。
    KamataEngine::Vector3 normal{0.0f, 0.0f, 0.0f};
    KamataEngine::Vector3 point{0.0f, 0.0f, 0.0f};
    float penetrationDepth = 0.0f;
};

enum class TouchPhase {
    kEnter,
    kStay,
    kExit,
};

inline float Dot(
    const KamataEngine::Vector3& lhs,
    const KamataEngine::Vector3& rhs) {

    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline bool ComputeAABBContact(
    const MapAABB& moving,
    const MapAABB& fixed,
    CollisionContact& outContact) {

    const float deltaX = moving.center.x - fixed.center.x;
    const float deltaY = moving.center.y - fixed.center.y;
    const float deltaZ = moving.center.z - fixed.center.z;

    float overlapX =
        moving.halfSize.x + fixed.halfSize.x - std::fabs(deltaX);
    float overlapY =
        moving.halfSize.y + fixed.halfSize.y - std::fabs(deltaY);
    float overlapZ =
        moving.halfSize.z + fixed.halfSize.z - std::fabs(deltaZ);

    const float epsilon = Physics::Config::kContactEpsilon;
    if (overlapX < -epsilon ||
        overlapY < -epsilon ||
        overlapZ < -epsilon) {
        return false;
    }

    // 浮點誤差造成的微小負值不應形成反向修正量。
    overlapX = max(0.0f, overlapX);
    overlapY = max(0.0f, overlapY);
    overlapZ = max(0.0f, overlapZ);

    outContact.penetrationDepth = overlapX;
    outContact.normal = {
        (deltaX < 0.0f) ? -1.0f : 1.0f,
        0.0f,
        0.0f,
    };

    // 深度相同時優先 Y，避免站在地面時被誤認成側面碰撞。
    if (overlapY <= outContact.penetrationDepth) {
        outContact.penetrationDepth = overlapY;
        outContact.normal = {
            0.0f,
            (deltaY < 0.0f) ? -1.0f : 1.0f,
            0.0f,
        };
    }

    if (overlapZ < outContact.penetrationDepth) {
        outContact.penetrationDepth = overlapZ;
        outContact.normal = {
            0.0f,
            0.0f,
            (deltaZ < 0.0f) ? -1.0f : 1.0f,
        };
    }

    outContact.point = {
        (moving.center.x + fixed.center.x) * 0.5f,
        (moving.center.y + fixed.center.y) * 0.5f,
        (moving.center.z + fixed.center.z) * 0.5f,
    };

    return true;
}

} // namespace Assets
