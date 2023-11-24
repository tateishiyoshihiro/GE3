#pragma once
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>
#include"WinApp.h"
class Input
{
public:
	//namespace省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="KeyNumber">キー番号</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	///	キートリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);
private://メンバ変数
	// キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keyboard;
	ComPtr<IDirectInput8> directInput;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	//WindowsAPI
	WinApp* winApp = nullptr;
};

