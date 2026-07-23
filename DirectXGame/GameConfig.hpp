#pragma once

namespace Block::Config {

inline constexpr float kBlockWidth = 2.0f;
inline constexpr float kBlockHeight = 2.0f;
inline constexpr float kBlockDepth = 2.0f;

} // namespace Block::Config

namespace Player::Config {

inline constexpr float kPlayerWidth = 2.0f;
inline constexpr float kPlayerHeight = 2.0f;
inline constexpr float kPlayerDepth = 2.0f;

} // namespace Player::Config

namespace Camera::Config {

inline constexpr float kCameraHeight = 5.0f;
inline constexpr float kCameraDistance = 15.0f;
inline constexpr float kVelocityBias = 0.3f;
inline constexpr float kInterpolationRate = 0.1f;

} // namespace Camera::Config

namespace Physics::Config {

// 接觸容差：讓「剛好貼住」也能被視為接觸。
inline constexpr float kContactEpsilon = 0.005f;

// 接地探針只向下延伸一小段，不會把遠處地板誤判成接地。
inline constexpr float kGroundProbeDistance = 0.03f;

// 每個碰撞子步驟允許的最大移動量。
// 這個值應小於最薄碰撞體厚度的一半。
inline constexpr float kMaxMovementPerSubstep = 0.20f;

} // namespace Physics::Config

namespace World::Config {

// 以下皆為約 60 FPS 下的「每幀」數值。
inline constexpr float kAcceleration = 0.04f;
inline constexpr float kAttenuation = 0.20f;
inline constexpr float kLimitRunSpeed = 0.30f;
inline constexpr float kGravityAcceleration = 0.06f;
inline constexpr float kLimitFallSpeed = 0.80f;
inline constexpr float kJumpAcceleration = 0.98f;//0.72f;
inline constexpr float kBounceSpeed = 0.95f;
inline constexpr float kTimeTurn = 0.18f;

} // namespace World::Config
