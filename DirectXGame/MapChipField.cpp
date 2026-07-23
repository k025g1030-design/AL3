#include "MapChipField.hpp"

#include <algorithm>
#include <cmath>

#include "MapObjectFactory.hpp"

namespace Assets {

void MapChipField::ResetData() {
    Finalize();
    stageData_ = {};
    mapChipWidth_ = 0.0f;
    mapChipHeight_ = 0.0f;
}

void MapChipField::LoadData(const std::string& filename) {
    // 防止重复Load时出生点累积。
    stageData_.enemyRespawnSnapshot.clear();
    stageData_.playerRespawnSnapshot = {0.0f, 0.0f, 0.0f};

    stageData_.data = ReadMapChip_(filename);
    stageData_.kNumBlockVertical =
        static_cast<uint32_t>(stageData_.data.size());

    stageData_.kNumBlockHorizontal = 0;
    for (const auto& row : stageData_.data) {
        stageData_.kNumBlockHorizontal = max(
            stageData_.kNumBlockHorizontal,
            static_cast<uint32_t>(row.size()));
    }

    for (uint32_t y = 0; y < stageData_.kNumBlockVertical; ++y) {
        for (uint32_t x = 0; x < stageData_.kNumBlockHorizontal; ++x) {
            const MapChipType type = GetMapChipTypeByIndex(x, y);

            if (type == MapChipType::kPlayerRespawn) {
                stageData_.playerRespawnSnapshot =
                    GetMapChipPositionByIndex(x, y);
            } else if (type == MapChipType::kEnemyRespawn) {
                stageData_.enemyRespawnSnapshot.push_back(
                    GetMapChipPositionByIndex(x, y));
            }
        }
    }

    mapChipWidth_ =
        stageData_.kNumBlockHorizontal * Block::Config::kBlockWidth;
    mapChipHeight_ =
        stageData_.kNumBlockVertical * Block::Config::kBlockHeight;
}

void MapChipField::BuildObjects(MapObjectFactory& factory) {
    for (auto& object : objects_) {
        object->Finalize();
    }
    objects_.clear();

    objectGrid_.assign(
        stageData_.kNumBlockVertical,
        std::vector<MapObject*>(stageData_.kNumBlockHorizontal, nullptr));

    for (uint32_t y = 0; y < stageData_.kNumBlockVertical; ++y) {
        for (uint32_t x = 0; x < stageData_.kNumBlockHorizontal; ++x) {
            const MapChipType type = GetMapChipTypeByIndex(x, y);
            const KamataEngine::Vector3 position =
                GetMapChipPositionByIndex(x, y);

            std::unique_ptr<MapObject> object = factory.Create(type, position);
            if (!object) {
                continue;
            }

            MapObject* rawObject = object.get();
            objects_.push_back(std::move(object));
            objectGrid_[y][x] = rawObject;
        }
    }
}

void MapChipField::UpdateObjects() {
    for (auto& object : objects_) {
        object->Update();
    }
}

void MapChipField::DrawObjects(const KamataEngine::Camera* camera) {
    for (auto& object : objects_) {
        object->Draw(camera);
    }
}

void MapChipField::Finalize() {
    for (auto& object : objects_) {
        object->Finalize();
    }

    objects_.clear();
    objectGrid_.clear();
}

std::vector<MapObject*> MapChipField::QueryNearby(const MapAABB& bounds) const {
    std::vector<MapObject*> result;

    if (objectGrid_.empty()) {
        return result;
    }

    const float blockWidth = Block::Config::kBlockWidth;
    const float blockHeight = Block::Config::kBlockHeight;
    const float epsilon = Physics::Config::kContactEpsilon;

    // MapObject 的世界座標是格子中心（x * blockWidth），不是左下角。
    // 因此查詢範圍必須再加入半個格子的候選寬度。
    const int minCellX = static_cast<int>(std::ceil(
        (bounds.center.x - bounds.halfSize.x -
         blockWidth * 0.5f - epsilon) /
        blockWidth));
    const int maxCellX = static_cast<int>(std::floor(
        (bounds.center.x + bounds.halfSize.x +
         blockWidth * 0.5f + epsilon) /
        blockWidth));

    // CSV的Y从上往下，世界坐标Y从下往上，所以查询时转换一次。
    const int minWorldCellY = static_cast<int>(std::ceil(
        (bounds.center.y - bounds.halfSize.y -
         blockHeight * 0.5f - epsilon) /
        blockHeight));
    const int maxWorldCellY = static_cast<int>(std::floor(
        (bounds.center.y + bounds.halfSize.y +
         blockHeight * 0.5f + epsilon) /
        blockHeight));

    for (int worldCellY = minWorldCellY;
         worldCellY <= maxWorldCellY;
         ++worldCellY) {

        const int csvY =
            static_cast<int>(stageData_.kNumBlockVertical) - 1 - worldCellY;

        if (csvY < 0 ||
            csvY >= static_cast<int>(stageData_.kNumBlockVertical)) {
            continue;
        }

        for (int x = minCellX; x <= maxCellX; ++x) {
            if (x < 0 ||
                x >= static_cast<int>(stageData_.kNumBlockHorizontal)) {
                continue;
            }

            MapObject* object = objectGrid_[csvY][x];
            if (object != nullptr && object->IsActive()) {
                result.push_back(object);
            }
        }
    }

    return result;
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t x, uint32_t y) const {
    if (y >= stageData_.data.size() || x >= stageData_.data[y].size()) {
        return MapChipType::kBlank;
    }

    return stageData_.data[y][x];
}

KamataEngine::Vector3 MapChipField::GetMapChipPositionByIndex(
    uint32_t x,
    uint32_t y) const {

    return {
        Block::Config::kBlockWidth * x,
        Block::Config::kBlockHeight *
            (GetNumBlockVertical() - 1 - y),
        0.0f,
    };
}

const std::vector<std::unique_ptr<MapObject>>&
MapChipField::GetObjects() const {
    return objects_;
}

uint32_t MapChipField::GetNumBlockVertical() const {
    return stageData_.kNumBlockVertical;
}

uint32_t MapChipField::GetNumBlockHorizontal() const {
    return stageData_.kNumBlockHorizontal;
}

KamataEngine::Vector3 MapChipField::GetPlayerRespawnPosition() const {
    return stageData_.playerRespawnSnapshot;
}

const std::vector<KamataEngine::Vector3>&
MapChipField::GetEnemyRespawnPositions() const {
    return stageData_.enemyRespawnSnapshot;
}

float MapChipField::GetMapChipWidth() const {
    return mapChipWidth_;
}

float MapChipField::GetMapChipHeight() const {
    return mapChipHeight_;
}

std::string MapChipField::Trim_(const std::string& value) const {
    size_t start = 0;
    while (start < value.size() &&
           std::isspace(static_cast<unsigned char>(value[start]))) {
        ++start;
    }

    size_t end = value.size();
    while (end > start &&
           std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }

    return value.substr(start, end - start);
}

bool MapChipField::IsInteger_(const std::string& value) const {
    if (value.empty()) {
        return false;
    }

    size_t index = 0;
    if (value[0] == '-' || value[0] == '+') {
        ++index;
    }

    if (index == value.size()) {
        return false;
    }

    for (; index < value.size(); ++index) {
        if (!std::isdigit(static_cast<unsigned char>(value[index]))) {
            return false;
        }
    }

    return true;
}

std::vector<std::vector<MapChipType>> MapChipField::ReadMapChip_(
    const std::string& filename) const {

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open CSV file: " + filename);
    }

    std::vector<std::vector<MapChipType>> matrix;
    std::string line;
    bool firstLine = true;

    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            if (line.size() >= 3 &&
                static_cast<unsigned char>(line[0]) == 0xEF &&
                static_cast<unsigned char>(line[1]) == 0xBB &&
                static_cast<unsigned char>(line[2]) == 0xBF) {
                line = line.substr(3);
            }
        }

        std::vector<MapChipType> row;
        std::stringstream stream(line);
        std::string cell;

        while (std::getline(stream, cell, ',')) {
            cell = Trim_(cell);

            if (!IsInteger_(cell)) {
                row.push_back(MapChipType::kBlank);
                continue;
            }

            row.push_back(
                static_cast<MapChipType>(std::stoi(cell)));
        }

        if (!row.empty()) {
            matrix.push_back(std::move(row));
        }
    }

    return matrix;
}

} // namespace Assets
