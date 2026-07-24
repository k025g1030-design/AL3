#pragma once

namespace Block::Config {
    inline constexpr float kBlockWidth = 2.0f;
    inline constexpr float kBlockHeight = 2.0f;
}

namespace Player::Config {
    // プレイヤーの幅
    inline constexpr float kPlayerWidth = 2.0f;
    // プレイヤーの高さ
    inline constexpr float kPlayerHeight = 2.0f;

}

namespace Camera::Config {
    // カメラの高さ
    inline constexpr float kCameraHeight = 5.0f;
    // カメラの距離
    inline constexpr float kCameraDistance = 15.0f;

    inline constexpr float kVelocityBias = 0.3f;

    inline constexpr float kInterpolationRate = 0.1f;

    // 強制スクロール時の1フレームあたりの移動量
    inline constexpr float kForcedScrollSpeed = 0.10f;
}

namespace World::Config {
    // 加速度
    inline constexpr float kAcceleration = 0.1f;
    // 減速率
    inline constexpr float kAttenuation = 0.3f;
    // 走る速度の上限
    inline constexpr float kLimitRunSpeed = 1.2f;
    // 重力加速度 
    inline constexpr float kGravityAcceleration = 0.2f;
    // 落下速度の上限
    inline constexpr float kLimitFallSpeed = 3.0f;
    // ジャンプの初速
    inline constexpr float kJumpAcceleration = 1.8f;
    // ターンにかかる時間
    inline constexpr float kTimeTurn = 0.3f;

    // 挟まれ死亡演出
    inline constexpr float kDeathRotationSpeed = 0.15f;
    inline constexpr float kDeathShrinkSpeed = 0.02f;
    inline constexpr float kDeathFallSpeed = 0.20f;
}
