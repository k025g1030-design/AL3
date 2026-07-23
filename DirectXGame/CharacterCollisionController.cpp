#include "CharacterCollisionController.hpp"

#include <algorithm>
#include <cmath>
#include <unordered_set>

#include "GameConfig.hpp"

namespace Assets {

void CharacterCollisionController::Initialize(
    const KamataEngine::Vector3& colliderHalfSize,
    const KamataEngine::Vector3& colliderCenterOffset) {

    colliderHalfSize_ = {
        max(colliderHalfSize.x, Physics::Config::kContactEpsilon),
        max(colliderHalfSize.y, Physics::Config::kContactEpsilon),
        max(colliderHalfSize.z, Physics::Config::kContactEpsilon),
    };
    colliderCenterOffset_ = colliderCenterOffset;
    activeContacts_.clear();
}

void CharacterCollisionController::Move(
    MapChipField& field,
    IMapActor& actor) {

    const KamataEngine::Vector3 initialVelocity = actor.GetVelocity();
    const float maximumMovement = max( 
        std::fabs(initialVelocity.x), max(
        std::fabs(initialVelocity.y),
        std::fabs(initialVelocity.z))
    );

    const int substepCount = max(
        1,
        static_cast<int>(std::ceil(
            maximumMovement /
            Physics::Config::kMaxMovementPerSubstep)));

    std::unordered_map<MapObject*, CollisionContact> frameContacts;
    std::unordered_set<MapObject*> dispatchedThisFrame;

    for (int step = 0; step < substepCount; ++step) {
        const KamataEngine::Vector3 velocity = actor.GetVelocity();
        KamataEngine::Vector3 position = actor.GetPosition();

        position.x += velocity.x / substepCount;
        position.y += velocity.y / substepCount;
        position.z += velocity.z / substepCount;
        actor.SetPosition(position);

        MapAABB actorCollider = MakeCollider_(actor);
        const std::vector<MapObject*> nearby =
            field.QueryNearby(actorCollider);

        for (MapObject* object : nearby) {
            if (object == nullptr ||
                !object->IsActive() ||
                !object->HasCollider()) {
                continue;
            }

            CollisionContact contact;
            if (!ComputeAABBContact(
                    actorCollider,
                    object->GetCollider(),
                    contact)) {
                continue;
            }

            if (object->IsSolid()) {
                ResolveSolidContact_(actor, contact);
                actorCollider = MakeCollider_(actor);
            }

            frameContacts[object] = contact;

            if (dispatchedThisFrame.insert(object).second) {
                const TouchPhase phase =
                    activeContacts_.count(object) != 0
                    ? TouchPhase::kStay
                    : TouchPhase::kEnter;

                object->DispatchTouch({actor, contact, phase});

                // Teleport / Bounce 等 Component 可能改變角色狀態。
                actorCollider = MakeCollider_(actor);
            }
        }
    }

    // 子步驟只負責不穿透；整幀結束後才建立正式接觸快照。
    std::unordered_map<MapObject*, CollisionContact> finalContacts =
        CollectContacts_(field, actor);

    for (const auto& [object, contact] : finalContacts) {
        if (dispatchedThisFrame.insert(object).second) {
            const TouchPhase phase =
                activeContacts_.count(object) != 0
                ? TouchPhase::kStay
                : TouchPhase::kEnter;

            object->DispatchTouch({actor, contact, phase});
        }
    }

    // 上一幀有、這一幀沒有：Exit。
    for (const auto& [object, previousContact] : activeContacts_) {
        if (finalContacts.count(object) == 0 && object != nullptr) {
            object->DispatchTouch({
                actor,
                previousContact,
                TouchPhase::kExit,
            });
        }
    }

    // 同一幀穿過 Trigger 時仍產生完整 Enter -> Exit。
    for (const auto& [object, transientContact] : frameContacts) {
        if (activeContacts_.count(object) == 0 &&
            finalContacts.count(object) == 0 &&
            object != nullptr) {

            object->DispatchTouch({
                actor,
                transientContact,
                TouchPhase::kExit,
            });
        }
    }

    activeContacts_ = std::move(finalContacts);

    // Grounded 是碰撞結果的快照，一幀只在此處寫入一次。
    actor.SetGrounded(IsGroundSupported_(field, actor));
}

void CharacterCollisionController::ClearContacts() {
    activeContacts_.clear();
}

MapAABB CharacterCollisionController::MakeCollider_(
    const IMapActor& actor) const {

    const KamataEngine::Vector3 position = actor.GetPosition();

    return {
        {
            position.x + colliderCenterOffset_.x,
            position.y + colliderCenterOffset_.y,
            position.z + colliderCenterOffset_.z,
        },
        colliderHalfSize_,
    };
}

void CharacterCollisionController::ResolveSolidContact_(
    IMapActor& actor,
    const CollisionContact& contact) const {

    KamataEngine::Vector3 position = actor.GetPosition();
    position.x += contact.normal.x * contact.penetrationDepth;
    position.y += contact.normal.y * contact.penetrationDepth;
    position.z += contact.normal.z * contact.penetrationDepth;
    actor.SetPosition(position);

    KamataEngine::Vector3 velocity = actor.GetVelocity();
    const float inwardSpeed = Dot(velocity, contact.normal);

    if (inwardSpeed < 0.0f) {
        velocity.x -= contact.normal.x * inwardSpeed;
        velocity.y -= contact.normal.y * inwardSpeed;
        velocity.z -= contact.normal.z * inwardSpeed;
        actor.SetVelocity(velocity);
    }
}

std::unordered_map<MapObject*, CollisionContact>
CharacterCollisionController::CollectContacts_(
    MapChipField& field,
    const IMapActor& actor) const {

    std::unordered_map<MapObject*, CollisionContact> result;
    const MapAABB collider = MakeCollider_(actor);

    for (MapObject* object : field.QueryNearby(collider)) {
        if (object == nullptr ||
            !object->IsActive() ||
            !object->HasCollider()) {
            continue;
        }

        CollisionContact contact;
        if (ComputeAABBContact(
                collider,
                object->GetCollider(),
                contact)) {
            result[object] = contact;
        }
    }

    return result;
}

bool CharacterCollisionController::IsGroundSupported_(
    MapChipField& field,
    const IMapActor& actor) const {

    if (actor.GetVelocity().y >
        Physics::Config::kContactEpsilon) {
        return false;
    }

    const MapAABB actorCollider = MakeCollider_(actor);
    MapAABB groundProbe = actorCollider;

    // 只把 AABB 的底部向下延伸，頂部位置保持不變。
    groundProbe.center.y -=
        Physics::Config::kGroundProbeDistance * 0.5f;
    groundProbe.halfSize.y +=
        Physics::Config::kGroundProbeDistance * 0.5f;

    for (MapObject* object : field.QueryNearby(groundProbe)) {
        if (object == nullptr ||
            !object->IsActive() ||
            !object->HasCollider() ||
            !object->IsSolid()) {
            continue;
        }

        const MapAABB& fixed = object->GetCollider();

        const float overlapX =
            actorCollider.halfSize.x + fixed.halfSize.x -
            std::fabs(actorCollider.center.x - fixed.center.x);
        const float overlapZ =
            actorCollider.halfSize.z + fixed.halfSize.z -
            std::fabs(actorCollider.center.z - fixed.center.z);

        if (overlapX <= Physics::Config::kContactEpsilon ||
            overlapZ <= Physics::Config::kContactEpsilon) {
            continue;
        }

        const float actorBottom =
            actorCollider.center.y - actorCollider.halfSize.y;
        const float objectTop =
            fixed.center.y + fixed.halfSize.y;
        const float gap = actorBottom - objectTop;

        if (gap >= -Physics::Config::kContactEpsilon &&
            gap <= Physics::Config::kGroundProbeDistance) {
            return true;
        }
    }

    return false;
}

} // namespace Assets
