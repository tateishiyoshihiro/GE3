#include "Input.h"
#include<cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void Input::Initialize(WinApp* winApp)
{
    //借りてきたwinAppのインスタンスを記録する
    this->winApp = winApp;
    HRESULT result;
    // DirectInputの初期化
    result = DirectInput8Create(
        winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

    
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    // 入力データ形式のセット
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(result));
    // 排他制御レベルのセット
    result = keyboard->SetCooperativeLevel(
        winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));

}

void Input::Update()
{
    HRESULT result;
    //前回のキー入力を保存
    memcpy(keyPre, key, sizeof(key));
    // キーボード情報の取得開始
    keyboard->Acquire();
    // 全キーの入力状態を取得する
    keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE KeyNumber)
{
    //指定キーを押していればtrueを返す
    if (key[KeyNumber]) {
        return true;
    }
    //そうでなかったらfalseを返す
    return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
    //指定キーを押していればtrueを返す
    if (key[keyNumber]&& keyPre[keyNumber]==0) {
        return true;
    }
    //そうでなかったらfalseを返す
    return false;
}
