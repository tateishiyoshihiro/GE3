#pragma once
#include"DirectXCommon.h"
#include<d3d12.h>
#include<wrl.h>
#include"SpriteCommon.h"
//単体
class Sprite
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon, SpriteCommon* common);
	void Draw();
private:
	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* common_ = nullptr;
	ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
};

