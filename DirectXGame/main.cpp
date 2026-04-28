#include <Windows.h>
#include "KamataEngine.h"


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	KamataEngine::Initialize(L"GC2B_08_ラ_ケツブン");
	KamataEngine::DirectXCommon* dxCommon = KamataEngine::DirectXCommon::GetInstance();

	uint32_t textureHandle_ = KamataEngine::TextureManager::Load("uvChecker.png");

	KamataEngine::Sprite* sprite_ = KamataEngine::Sprite::Create(textureHandle_, {100, 50});

	while (true) {
		if (KamataEngine::Update()) {
			break;
		}

		dxCommon->PreDraw();

		sprite_->PreDraw();


		sprite_->Draw();

		sprite_->PostDraw();

		dxCommon->PostDraw();
	}
	delete sprite_;
	KamataEngine::Finalize();
	
	return 0;
}
