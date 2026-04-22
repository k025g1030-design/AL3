#include <Windows.h>
#include "KamataEngine.h"


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	KamataEngine::Initialize(L"GC2B_08_ラ_ケツブン");
	KamataEngine::DirectXCommon* dxCommon = KamataEngine::DirectXCommon::GetInstance();


	while (true) {
		if (KamataEngine::Update()) {
			break;
		}

		dxCommon->PreDraw();

		dxCommon->PostDraw();
	}
	KamataEngine::Finalize();
	
	return 0;
}
