#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.hpp"
#include "TitleScene.hpp"

namespace {
enum class Scene {
    kTitle,
    kGame,
};
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
    KamataEngine::Initialize(L"GC2B_08_ラ_ケツブン");
    KamataEngine::DirectXCommon* dxCommon = KamataEngine::DirectXCommon::GetInstance();

    Scene scene = Scene::kTitle;
    Game::TitleScene* titleScene = new Game::TitleScene();
    Game::GameScene* gameScene = nullptr;
    titleScene->Initialize();
    
    while (true) {
        if (KamataEngine::Update()) {
            break;
        }
        switch (scene) {
        case Scene::kTitle:
            titleScene->Update();
            if (titleScene->IsFinished()) {
                titleScene->Finalize();
                delete titleScene;
                titleScene = nullptr;
                gameScene = new Game::GameScene();
                gameScene->Initialize();
                scene = Scene::kGame;
            }
            break;
        case Scene::kGame:
            gameScene->Update();
            if (gameScene->IsFinished()) {
                gameScene->Finalize();
                delete gameScene;
                gameScene = nullptr;
                titleScene = new Game::TitleScene();
                titleScene->Initialize();
                scene = Scene::kTitle;
            }
            break;
        }

        dxCommon->PreDraw();
        switch (scene) {
        case Scene::kTitle:
            titleScene->Draw();
            break;
        case Scene::kGame:
            gameScene->Draw();
            break;
        }

        KamataEngine::AxisIndicator::GetInstance()->Draw();

        dxCommon->PostDraw();
    }
    if (titleScene != nullptr) {
        titleScene->Finalize();
    }
    if (gameScene != nullptr) {
        gameScene->Finalize();
    }
    delete titleScene;
    delete gameScene;
    KamataEngine::Finalize();
    
    return 0;
}
