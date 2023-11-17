﻿#pragma once
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>
class Input
{
public:
	//namespace省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	//初期化
	void Initialize(HINSTANCE hInstance,HWND hwnd);
	//更新
	void Update();
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="KeyNumber">キー番号</param>
	/// <returns></returns>
	bool PushKey(BYTE KeyNumber);
private://メンバ変数
	// キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256] = {};
};

