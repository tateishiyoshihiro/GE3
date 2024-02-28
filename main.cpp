#include"Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"imGuiManager.h"
#include "TextureManager.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma region WindowsAPI初期化処理
	//ポインタ
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	//入力の初期化
	winApp = new WinApp();
	winApp->Initialize();
#pragma endregion
#pragma region DirectX初期化処理
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	// DirectX初期化処理　ここまで
#pragma endregion
	//入力の初期化
	input = new Input();
	input->Initialize(winApp);
	//ImGui
	imGuiManager* imgui = imGuiManager::Create();
	imGuiManager::Initialize(winApp->GetHwnd(), dxCommon);

	TextureManager::GetInstance()->Initialize(dxCommon);
	TextureManager::GetInstance()->LoadTexture(L"Resources/mario.jpg");
	//スプライト顧問
	SpriteCommon* spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);
	//スプライト
	std::vector<Sprite*>sprites;
	for (uint32_t i = 0; i < 5; ++i) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteCommon, L"Resources/mario.jpg");
		sprite->SetPosition({ (float)(i * 1.0f),0.0f });
		sprites.push_back(sprite);
	}






	// ゲームループ
	while (true) {
		if (winApp->Update() == true)
		{
			break;
		}
		imGuiManager::NewFrame();
		imgui->ShowDemo();
#pragma region Update


		//入力の更新
		input->Update();

		spriteCommon->SpritePreDraw();
		/*DirectX::XMFLOAT2 pos = sprite->GetPosition();
		pos.x += 0.01f;
		sprite->SetPosition(pos);*/
		/*DirectX::XMFLOAT2 size = sprite->GetSize();
		size.x += 0.01f;
		size.y += 0.01f;
		sprite->SetSize(size);
		sprite->Update();*/

		for (uint32_t i = 0; i < 5; ++i) {
			sprites[i]->Update();
		}

#pragma endregion

		imGuiManager::CreateCommand();

#pragma region Draw


		dxCommon->PreDraw();


		for (uint32_t i = 0; i < 5; ++i) {
			sprites[i]->Draw();
		}
		//sprite->Draw();
#pragma endregion

		// ４．描画コマンドここまで
		imGuiManager::CommandsExcute(dxCommon->GetCommandList());
		dxCommon->Postdraw();
	}
	//入力解放
	for (uint32_t i = 0; i < 5; ++i) {
		delete sprites[i];
	}
	delete spriteCommon;
	delete imgui;
	delete input;
	delete dxCommon;
	TextureManager::GetInstance()->Finalize();
	//WindowsAPIの終了処理
	winApp->Finalize();
	delete winApp;
	return 0;
}
