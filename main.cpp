#include"Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

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
    
    // ゲームループ
   while (true) {
       if (winApp->Update()==true)
       {
           break;
       }
       //入力の更新
       input->Update();

       dxCommon->PreDraw();
       // ４．描画コマンドここまで
       dxCommon->Postdraw();
   }
    
    //入力解放
    delete input;
    delete winApp;
    delete dxCommon;
    //WindowsAPIの終了処理
    winApp->Finalize();
    return 0;
}
