#pragma once
#include <cstdint>      
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include "Box.hpp"

namespace Assets {
    static inline const float kBlockWidth = 2.0f;  
    static inline const float kBlockHeight = 2.0f;

    enum class MapChipType : uint32_t {
        kBlank = 0,
        kBlock = 1,
    };

    struct MapChipData {
        std::vector<std::vector<MapChipType>> data;
        uint32_t kNumBlockVertical = 0;
        uint32_t kNumBlockHorizontal = 0;
    };

    class MapChipField {

    public:
        void ResetData();
        void LoadData();

        void Finalize() {
            for (auto block : blocks_) {
                block->Finalize();
                delete block;
            }
            blocks_.clear();
        }

        MapChipType GetMapChipTypeByIndex(uint32_t x, uint32_t y) const {
            if (y < stageData_.data.size() && x < stageData_.data[y].size()) {
                return stageData_.data[y][x];
            } else {
                return MapChipType::kBlank; // 範囲外は空白とみなす
            }
        }

        KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t x, uint32_t y) const {
            return KamataEngine::Vector3(
                kBlockWidth * x, kBlockHeight * (GetNumBlockVertical() - 1 - y), 0.0f
            );
        }

        void AddBlock(Box* block) {
            blocks_.push_back(block);
        }
        
        std::vector<Box*> GetBlocks() const {
            return blocks_;
        }

        uint32_t GetNumBlockVertical() const {
            return stageData_.kNumBlockVertical;
        }
        uint32_t GetNumBlockHorizontal() const {
            return stageData_.kNumBlockHorizontal;
        }

        
    
    private:
        std::string Trim_(const std::string& s) {
            size_t start = 0;
            while (start < s.size() && std::isspace((unsigned char)s[start])) start++;

            size_t end = s.size();
            while (end > start && std::isspace((unsigned char)s[end - 1])) end--;

            return s.substr(start, end - start);
        }

        bool IsInteger_(const std::string& s) {
            if (s.empty()) return false;

            size_t i = 0;
            if (s[0] == '-' || s[0] == '+') i++;

            if (i == s.size()) return false;

            for (; i < s.size(); i++) {
                if (!std::isdigit((unsigned char)s[i])) return false;
            }

            return true;
        }

        std::vector<std::vector<MapChipType>> ReadMapChip_(const std::string& filename) {
            std::vector<std::vector<MapChipType>> matrix;
            std::ifstream file(filename);

            if (!file.is_open()) {
                throw std::runtime_error("Cannot open CSV file: " + filename);
            }

            std::string line;

            if (std::getline(file, line)) {
                if (line.size() >= 3 &&
                    (unsigned char)line[0] == 0xEF &&
                    (unsigned char)line[1] == 0xBB &&
                    (unsigned char)line[2] == 0xBF) {
                    line = line.substr(3);
                }
            } else {
                return matrix; // is null
            }

            auto processLine = [&](const std::string& ln) {
                std::vector<MapChipType> row;
                std::stringstream ss(ln);
                std::string cell;

                while (std::getline(ss, cell, ',')) {
                    cell = Trim_(cell);
                    if (cell.empty()) continue;             // 
                    if (IsInteger_(cell)) {
                        row.push_back(static_cast<MapChipType>(std::stoi(cell)));
                    }
                }

                if (!row.empty()) matrix.push_back(row);
            };

            processLine(line);

            while (std::getline(file, line)) {
                processLine(line);
            }
            
            return matrix;
        }

    private:
        MapChipData stageData_;

        std::vector<Box*> blocks_;

    };

} // namespace Assets
