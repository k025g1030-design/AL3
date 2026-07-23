#pragma once

#include <cassert>
#include <cstdint>

#include "KamataEngine.h"
#include "Math.hpp"

namespace Assets {

// Box只负责一个地图物件的可视化。
// Model和Texture由场景的ResourceManager持有，Box只保存非拥有引用。
class Box {
public:
    void Initialize(
        KamataEngine::Model* model,
        uint32_t textureHandle,
        const KamataEngine::Vector3& position);

    void Update();
    void Draw(const KamataEngine::Camera* camera);

    // 只解除引用，不释放共享资源。
    void Finalize();

    void SetPosition(const KamataEngine::Vector3& position);
    const KamataEngine::Vector3& GetPosition() const;

    void SetScale(const KamataEngine::Vector3& scale);
    const KamataEngine::Vector3& GetScale() const;

    void SetRotation(const KamataEngine::Vector3& rotation);
    const KamataEngine::Vector3& GetRotation() const;

private:
    void ApplyTransform_();

private:
    KamataEngine::Model* model_ = nullptr; // non-owning
    KamataEngine::WorldTransform worldTransform_{};
    uint32_t textureHandle_ = 0;           // non-owning handle
};

} // namespace Assets
