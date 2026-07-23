#include "Player.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace Actor {

void Player::Initialize(
    KamataEngine::Model* model,
    const KamataEngine::Vector3& position) {

    assert(model != nullptr);
    model_ = model;

    worldTransform_.Initialize();
    worldTransform_.scale_ = {2.0f, 2.0f, 2.0f};
    worldTransform_.rotation_ = {
        0.0f,
        std::numbers::pi_v<float> / 2.0f,
        0.0f,
    };
    worldTransform_.translation_ = position;

    velocity_ = {0.0f, 0.0f, 0.0f};
    lrDirection_ = LRDirection::kRight;
    turnFirstRotationY_ = worldTransform_.rotation_.y;
    turnTimer_ = 0.0f;
    onGround_ = false;
    health_ = kMaxHealth_;

    SyncTransform();
}

void Player::Update() {
    KamataEngine::Input* input =
        KamataEngine::Input::GetInstance();

    if (onGround_) {
        float accelerationX = 0.0f;

        if (input->PushKey(DIK_A)) {
            accelerationX -= World::Config::kAcceleration;

            if (lrDirection_ != LRDirection::kLeft) {
                lrDirection_ = LRDirection::kLeft;
                StartTurn_();
            }
        }

        if (input->PushKey(DIK_D)) {
            accelerationX += World::Config::kAcceleration;

            if (lrDirection_ != LRDirection::kRight) {
                lrDirection_ = LRDirection::kRight;
                StartTurn_();
            }
        }

        if (accelerationX != 0.0f) {
            velocity_.x += accelerationX;
            velocity_.x = std::clamp(
                velocity_.x,
                -World::Config::kLimitRunSpeed,
                World::Config::kLimitRunSpeed);
        } else {
            velocity_.x *= 1.0f - World::Config::kAttenuation;

            if (std::fabs(velocity_.x) < 0.001f) {
                velocity_.x = 0.0f;
            }
        }

        // TriggerKey 只在按下的第一幀成立，跳躍速度也必須是賦值。
        if (input->TriggerKey(DIK_SPACE)) {
            velocity_.y = World::Config::kJumpAcceleration;
            onGround_ = false;
        }
    } else {
        velocity_.y -= World::Config::kGravityAcceleration;
        velocity_.y = max(
            velocity_.y,
            -World::Config::kLimitFallSpeed);
    }

    // 轉向只影響外觀，不能阻止 X/Y 的物理移動。
    UpdateTurn_();
}

void Player::SyncTransform() {
    ApplyTransform_();
}

void Player::Draw(const KamataEngine::Camera* camera) {
    if (model_ == nullptr || camera == nullptr) {
        return;
    }

    model_->Draw(worldTransform_, *camera);
}

void Player::Finalize() {
    model_ = nullptr;
}

void Player::ApplyDamage(int damage) {
    health_ = std::clamp(health_ - damage, 0, kMaxHealth_);
}

KamataEngine::Vector3 Player::GetPosition() const {
    return worldTransform_.translation_;
}

void Player::SetPosition(const KamataEngine::Vector3& position) {
    worldTransform_.translation_ = position;
}

KamataEngine::Vector3 Player::GetVelocity() const {
    return velocity_;
}

void Player::SetVelocity(const KamataEngine::Vector3& velocity) {
    velocity_ = velocity;
}

void Player::SetGrounded(bool grounded) {
    onGround_ = grounded;
}

KamataEngine::WorldTransform& Player::GetWorldTransform() {
    return worldTransform_;
}

const KamataEngine::WorldTransform& Player::GetWorldTransform() const {
    return worldTransform_;
}

bool Player::IsGrounded() const {
    return onGround_;
}

int Player::GetHealth() const {
    return health_;
}

void Player::StartTurn_() {
    turnFirstRotationY_ = worldTransform_.rotation_.y;
    turnTimer_ = World::Config::kTimeTurn;
}

void Player::UpdateTurn_() {
    if (turnTimer_ <= 0.0f) {
        return;
    }

    turnTimer_ = max(0.0f, turnTimer_ - 1.0f / 60.0f);

    const float destinationRotationYTable[] = {
        std::numbers::pi_v<float> / 2.0f,
        std::numbers::pi_v<float> * 3.0f / 2.0f,
    };

    const float destination =
        destinationRotationYTable[static_cast<int>(lrDirection_)];

    const float progress =
        1.0f - turnTimer_ / World::Config::kTimeTurn;

    worldTransform_.rotation_.y =
        turnFirstRotationY_ +
        (destination - turnFirstRotationY_) *
            EaseInOutBounce_(progress);
}

float Player::EaseOutBounce_(float t) const {
    if (t < 1.0f / 2.75f) {
        return 7.5625f * t * t;
    }

    if (t < 2.0f / 2.75f) {
        t -= 1.5f / 2.75f;
        return 7.5625f * t * t + 0.75f;
    }

    if (t < 2.5f / 2.75f) {
        t -= 2.25f / 2.75f;
        return 7.5625f * t * t + 0.9375f;
    }

    t -= 2.625f / 2.75f;
    return 7.5625f * t * t + 0.984375f;
}

float Player::EaseInOutBounce_(float t) const {
    return t < 0.5f
        ? (1.0f - EaseOutBounce_(1.0f - 2.0f * t)) / 2.0f
        : (1.0f + EaseOutBounce_(2.0f * t - 1.0f)) / 2.0f;
}

void Player::ApplyTransform_() {
    worldTransform_.matWorld_ = MathUtils::MakeAffineMatrix(
        worldTransform_.scale_,
        worldTransform_.rotation_,
        worldTransform_.translation_);

    worldTransform_.TransferMatrix();
}

} // namespace Actor
