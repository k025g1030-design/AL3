#pragma once

#include <unordered_map>

#include "MapChipField.hpp"
#include "MapObjectEvents.hpp"

namespace Assets {

// 角色移動與地圖碰撞的唯一負責者。
//
// Player 只產生 velocity，GameScene 只安排更新順序；
// 位置積分、分段移動、碰撞修正與接地結算全部集中在這裡。
class CharacterCollisionController {
public:
    void Initialize(
        const KamataEngine::Vector3& colliderHalfSize,
        const KamataEngine::Vector3& colliderCenterOffset =
            {0.0f, 0.0f, 0.0f});

    void Move(MapChipField& field, IMapActor& actor);

    // 更換或卸載地圖前呼叫，避免保留已失效的 MapObject 指標。
    void ClearContacts();

private:
    MapAABB MakeCollider_(const IMapActor& actor) const;

    void ResolveSolidContact_(
        IMapActor& actor,
        const CollisionContact& contact) const;

    std::unordered_map<MapObject*, CollisionContact> CollectContacts_(
        MapChipField& field,
        const IMapActor& actor) const;

    bool IsGroundSupported_(
        MapChipField& field,
        const IMapActor& actor) const;

private:
    KamataEngine::Vector3 colliderHalfSize_{0.5f, 0.5f, 0.5f};
    KamataEngine::Vector3 colliderCenterOffset_{0.0f, 0.0f, 0.0f};

    // 只跨幀保存仍接觸中的物件，用來產生 Enter / Stay / Exit。
    std::unordered_map<MapObject*, CollisionContact> activeContacts_{};
};

} // namespace Assets
