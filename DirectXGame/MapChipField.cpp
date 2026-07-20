#include "MapChipField.hpp"

namespace Assets {
    void MapChipField::ResetData() {
        stageData_.data.clear();
        stageData_.data.resize(GetNumBlockVertical());
        for (auto& row : stageData_.data) {
            row.resize(GetNumBlockHorizontal());
        }
    }

    void MapChipField::LoadData() {
        stageData_.data = ReadMapChip_("./Resources/datas/blocks.csv");
        stageData_.kNumBlockVertical = static_cast<uint32_t>(stageData_.data.size());
        if (!stageData_.data.empty()) {
            stageData_.kNumBlockHorizontal = static_cast<uint32_t>(stageData_.data[0].size());
        }
        // プレイヤーと敵のリスポーン位置を取得
        for (uint32_t y = 0; y < stageData_.kNumBlockVertical; ++y) {
            for (uint32_t x = 0; x < stageData_.kNumBlockHorizontal; ++x) {
                if (stageData_.data[y][x] == MapChipType::kPlayerRespawn) {
                    stageData_.playerRespawnSnapshot = GetMapChipPositionByIndex(x, y);
                } else if (stageData_.data[y][x] == MapChipType::kEnemyRespawn) {
                    stageData_.enemyRespawnSnapshot.push_back(GetMapChipPositionByIndex(x, y));
                }
            }
        }
        mapChipWidth_ = stageData_.kNumBlockHorizontal * Block::Config::kBlockWidth;
        mapChipHeight_ = stageData_.kNumBlockVertical * Block::Config::kBlockHeight;

    }
}