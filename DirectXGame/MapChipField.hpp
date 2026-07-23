#pragma once

#include <cctype>
#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "GameConfig.hpp"
#include "MapObject.hpp"

namespace Assets {

class MapObjectFactory;

enum class MapChipType : uint32_t {
    kBlank = 0,
    kBlock = 1,
    kPlayerRespawn = 2,
    kEnemyRespawn = 3,

    // Component Pattern示例类型
    kDamageFloor = 4,
    kBreakableBox = 5,
    kTeleporter = 6,
    kBounceFloor = 7,
};

struct MapChipData {
    std::vector<std::vector<MapChipType>> data{};
    uint32_t kNumBlockVertical = 0;
    uint32_t kNumBlockHorizontal = 0;
    KamataEngine::Vector3 playerRespawnSnapshot{0.0f, 0.0f, 0.0f};
    std::vector<KamataEngine::Vector3> enemyRespawnSnapshot{};
};

class MapChipField {
public:
    void ResetData();
    void LoadData(const std::string& filename = "./Resources/datas/blocks.csv");

    // 将CSV中的类型交给Factory，生成真正的MapObject。
    void BuildObjects(MapObjectFactory& factory);

    void UpdateObjects();
    void DrawObjects(const KamataEngine::Camera* camera);
    void Finalize();

    // 规则网格就是Broad Phase，只返回AABB附近格子的物件。
    std::vector<MapObject*> QueryNearby(const MapAABB& bounds) const;

    MapChipType GetMapChipTypeByIndex(uint32_t x, uint32_t y) const;
    KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t x, uint32_t y) const;

    const std::vector<std::unique_ptr<MapObject>>& GetObjects() const;

    uint32_t GetNumBlockVertical() const;
    uint32_t GetNumBlockHorizontal() const;

    KamataEngine::Vector3 GetPlayerRespawnPosition() const;
    const std::vector<KamataEngine::Vector3>& GetEnemyRespawnPositions() const;

    float GetMapChipWidth() const;
    float GetMapChipHeight() const;

private:
    std::string Trim_(const std::string& value) const;
    bool IsInteger_(const std::string& value) const;

    std::vector<std::vector<MapChipType>> ReadMapChip_(
        const std::string& filename) const;

private:
    MapChipData stageData_{};
    float mapChipWidth_ = 0.0f;
    float mapChipHeight_ = 0.0f;

    // objects_拥有物件生命周期。
    std::vector<std::unique_ptr<MapObject>> objects_{};

    // objectGrid_只保存非拥有指针，用于附近查询。
    // 当前示例假设一个CSV格子最多生成一个MapObject。
    std::vector<std::vector<MapObject*>> objectGrid_{};
};

} // namespace Assets
