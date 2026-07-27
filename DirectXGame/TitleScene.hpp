#pragma once

#include "KamataEngine.h"

namespace Game {

/// <summary>
/// スペースキーでゲームを開始するタイトルシーン
/// </summary>
class TitleScene {
public:
    void Initialize();
    void Update();
    void Draw();
    void Finalize();

    bool IsFinished() const { return finished_; }

private:
    KamataEngine::Camera camera_;
    KamataEngine::Model* titleModel_ = nullptr;
    KamataEngine::WorldTransform worldTransform_;
    bool finished_ = false;
};

} // namespace Game
