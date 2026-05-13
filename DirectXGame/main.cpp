#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.hpp"


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	KamataEngine::Initialize(L"GC2B_08_ラ_ケツブン");
	KamataEngine::DirectXCommon* dxCommon = KamataEngine::DirectXCommon::GetInstance();

    Game::GameScene gameScene;

    gameScene.Initialize();

	
	while (true) {
		if (KamataEngine::Update()) {
			break;
		}
        gameScene.Update();



		dxCommon->PreDraw();

        gameScene.Draw();

		KamataEngine::AxisIndicator::GetInstance()->Draw();

		dxCommon->PostDraw();
	}

    gameScene.Finalize();
	KamataEngine::Finalize();
	
	return 0;
}
