#include "MapChipField.hpp"
#include <cmath>
#include <limits>

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

    IndexSet MapChipField::GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position) const {
        const int xIndex = static_cast<int>(std::floor(
            (position.x + Block::Config::kBlockWidth / 2.0f) /
            Block::Config::kBlockWidth));
        const int yIndexFromBottom = static_cast<int>(std::floor(
            (position.y + Block::Config::kBlockHeight / 2.0f) /
            Block::Config::kBlockHeight));
        const int yIndex =
            static_cast<int>(GetNumBlockVertical()) - 1 - yIndexFromBottom;

        const uint32_t invalidIndex = (std::numeric_limits<uint32_t>::max)();
        IndexSet indexSet = { invalidIndex, invalidIndex };

        if (xIndex >= 0 && xIndex < static_cast<int>(GetNumBlockHorizontal())) {
            indexSet.xIndex = static_cast<uint32_t>(xIndex);
        }
        if (yIndex >= 0 && yIndex < static_cast<int>(GetNumBlockVertical())) {
            indexSet.yIndex = static_cast<uint32_t>(yIndex);
        }

        return indexSet;
    }

    Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) const {
        const KamataEngine::Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

        Rect rect;
        rect.left = center.x - Block::Config::kBlockWidth / 2.0f;
        rect.right = center.x + Block::Config::kBlockWidth / 2.0f;
        rect.bottom = center.y - Block::Config::kBlockHeight / 2.0f;
        rect.top = center.y + Block::Config::kBlockHeight / 2.0f;
        return rect;
    }
}
