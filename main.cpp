#include"Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"imGuiManager.h"
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
    dxCommon -> Initialize(winApp);
    // DirectX初期化処理　ここまで
#pragma endregion
    //入力の初期化
    input = new Input();
    input->Initialize(winApp);
    //ImGui
    imGuiManager* imgui = imGuiManager::Create();
    imGuiManager::Initialize(winApp->GetHwnd(), dxCommon);
    //スプライト顧問
    SpriteCommon* spriteCommon = new SpriteCommon();
    spriteCommon->Initialize(dxCommon);
    //スプライト
    Sprite* sprite = new Sprite();
    sprite->Initialize(dxCommon,spriteCommon);
    // ゲームループ
   while (true) {
       if (winApp->Update()==true)
       {
           break;
       }
       imGuiManager::NewFrame();
       imgui->ShowDemo();
       //入力の更新
       input->Update();
       imGuiManager::CreateCommand();
       dxCommon->PreDraw();
       sprite->Draw();
       // ４．描画コマンドここまで
       imGuiManager::CommandsExcute(dxCommon->GetCommandList());
       dxCommon->Postdraw();
   }
    //入力解放
    delete sprite;
    delete spriteCommon;
    delete imgui;
    delete input;
    delete dxCommon;
    //WindowsAPIの終了処理
    winApp->Finalize();
    delete winApp;
    return 0;
}
