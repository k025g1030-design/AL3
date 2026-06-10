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


    }
}