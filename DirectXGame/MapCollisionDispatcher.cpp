#include "MapCollisionDispatcher.hpp"

namespace Assets {

    void MapCollisionDispatcher::ProcessActor(
        MapChipField& field,
        IMapActor& actor,
        MapAABB& actorCollider) {

        actor.SetGrounded(false);

        std::unordered_set<MapObject*> currentContacts;

        // 如果玩家速度很快，正式项目应把这里的查询范围换成Swept AABB。
        const std::vector<MapObject*> nearbyObjects =
            field.QueryNearby(actorCollider);

        for (MapObject* object : nearbyObjects) {
            if (object == nullptr ||
                !object->IsActive() ||
                !object->HasCollider()) {
                continue;
            }

            CollisionContact contact{};
            if (!ComputeAABBContact(
                actorCollider,
                object->GetCollider(),
                contact)) {
                continue;
            }

            currentContacts.insert(object);

            if (object->IsSolid()) {
                ResolveSolidContact_(actor, actorCollider, contact);

                constexpr float kGroundTolerance = 0.02f;

                const float actorBottom =
                    actorCollider.center.y -
                    actorCollider.halfSize.y;

                const float objectTop =
                    object->GetCollider().center.y +
                    object->GetCollider().halfSize.y;

                const bool touchingTop =
                    std::fabs(actorBottom - objectTop) <=
                    kGroundTolerance;

                // 法线判断＋位置判断，任意一个成立即可
                if (contact.normal.y > 0.7f ||
                    touchingTop) {

                    actor.SetGrounded(true);
                }
            }

            const TouchPhase phase =
                previousContacts_.count(object) > 0
                ? TouchPhase::kStay
                : TouchPhase::kEnter;

            object->DispatchTouch({ actor, contact, phase });

            // Teleport等Component可能已经直接修改Actor位置。
            // 示例假设Actor的位置就是Collider中心；有offset时请在Adapter中修正。
            actorCollider.center = actor.GetPosition();
        }

        for (MapObject* oldObject : previousContacts_) {
            if (oldObject == nullptr || currentContacts.count(oldObject) > 0) {
                continue;
            }

            CollisionContact emptyContact{};
            oldObject->DispatchTouch({
                actor,
                emptyContact,
                TouchPhase::kExit,
                });
        }

        previousContacts_ = std::move(currentContacts);
    }

    bool MapCollisionDispatcher::ProcessProjectile(
        MapChipField& field,
        IMapProjectile& projectile,
        const MapAABB& projectileCollider) {

        const std::vector<MapObject*> nearbyObjects =
            field.QueryNearby(projectileCollider);

        for (MapObject* object : nearbyObjects) {
            if (object == nullptr ||
                !object->IsActive() ||
                !object->HasCollider()) {
                continue;
            }

            CollisionContact contact{};
            if (!ComputeAABBContact(
                projectileCollider,
                object->GetCollider(),
                contact)) {
                continue;
            }

            object->DispatchShoot({ projectile, contact });
            return true;
        }

        return false;
    }

    void MapCollisionDispatcher::Activate(
        MapObject& object,
        IMapActor& actor) {

        object.DispatchActive({ actor });
    }

    void MapCollisionDispatcher::ClearContacts() {
        previousContacts_.clear();
    }

    void MapCollisionDispatcher::ResolveSolidContact_(
        IMapActor& actor,
        MapAABB& actorCollider,
        const CollisionContact& contact) {

        const KamataEngine::Vector3 correction = {
            contact.normal.x * contact.penetrationDepth,
            contact.normal.y * contact.penetrationDepth,
            contact.normal.z * contact.penetrationDepth,
        };

        KamataEngine::Vector3 position = actor.GetPosition();
        position.x += correction.x;
        position.y += correction.y;
        position.z += correction.z;
        actor.SetPosition(position);

        actorCollider.center.x += correction.x;
        actorCollider.center.y += correction.y;
        actorCollider.center.z += correction.z;

        KamataEngine::Vector3 velocity = actor.GetVelocity();
        const float normalVelocity = Dot(velocity, contact.normal);

        if (normalVelocity < 0.0f) {
            velocity.x -= contact.normal.x * normalVelocity;
            velocity.y -= contact.normal.y * normalVelocity;
            velocity.z -= contact.normal.z * normalVelocity;
            actor.SetVelocity(velocity);
        }
    }

} // namespace Assets
