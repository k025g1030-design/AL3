#include "Player.hpp"
#include <numbers>

namespace Actor {
    void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
        assert(model);
        assert(camera);
        model_ = model;
        camera_ = camera;

        // ワールドトランスフォームの初期化
        worldTransform_.Initialize();
        worldTransform_.scale_ = { 2.0f, 2.0f, 2.0f };
        //worldTransform_.scale_ = { 0.06f, 0.06f, 0.06f };
        worldTransform_.rotation_ = { 0, std::numbers::pi_v<float> / 2.0f, 0 };
        worldTransform_.translation_ = position;
        //worldTransform_.rotation_ = { 0, std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> / 2.0f };
    }
    void Player::Update() {
        KamataEngine::Input* input = KamataEngine::Input::GetInstance();
        if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
            KamataEngine::Vector3 acceleration = { 0.0f, 0.0f, 0.0f };
            if (input->PushKey(DIK_A)) {
                acceleration.x -= kAcceleration;
            }
            if (input->PushKey(DIK_D)) {
                acceleration.x += kAcceleration;
            }
            AddVelocity(acceleration);
            
        }
        Move();

        ApplyTransform_();
    }
    void Player::Draw() {

        model_->Draw(worldTransform_, *camera_);

    }
    void Player::Draw(const KamataEngine::Camera& camera) {

        model_->Draw(worldTransform_, camera);
    }
    void Player::Finalize() {
        model_ = nullptr;
    }
}
