#include "TitleScene.hpp"
#include "Math.hpp"
#include <cmath>
#include <numbers>

namespace Game {

void TitleScene::Initialize() {
    camera_.Initialize();
    camera_.translation_ = {0.0f, 0.0f, -12.0f};
    camera_.UpdateMatrix();

    // タイトル専用3Dモデルがないため、課題の指定どおりPlayerモデルで代用する。
    titleModel_ = KamataEngine::Model::CreateFromOBJ("player");
    worldTransform_.Initialize();
    worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
    finished_ = false;
}

void TitleScene::Update() {
    worldTransform_.rotation_.y += std::numbers::pi_v<float> / 180.0f;
    worldTransform_.translation_.y =
        0.35f * std::sin(worldTransform_.rotation_.y * 2.0f);
    worldTransform_.matWorld_ = MathUtils::MakeAffineMatrix(
        worldTransform_.scale_, worldTransform_.rotation_,
        worldTransform_.translation_);
    worldTransform_.TransferMatrix();

    if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
        finished_ = true;
    }
}

void TitleScene::Draw() {
    KamataEngine::Model::PreDraw();
    titleModel_->Draw(worldTransform_, camera_);
    KamataEngine::Model::PostDraw();
}

void TitleScene::Finalize() {
    delete titleModel_;
    titleModel_ = nullptr;
}

} // namespace Game
