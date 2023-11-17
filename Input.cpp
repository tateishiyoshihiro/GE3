﻿#include "Input.h"
#include<cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
    HRESULT result;
    // DirectInputの初期化
    ComPtr<IDirectInput8> directInput = nullptr;
    result = DirectInput8Create(
        hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

    
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    // 入力データ形式のセット
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(result));
    // 排他制御レベルのセット
    result = keyboard->SetCooperativeLevel(
        hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));

}

void Input::Update()
{
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
