#pragma once
#include<wrl.h>
#include <string>
#include<dxcapi.h>
#include <d3d12.h>
#include"DirectXCommon.h"
#include <../DirectXTex.h>

//共通
class SpriteCommon
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//初期化
	void Initialize(DirectXCommon*dxCommon);
	void SpritePreDraw();
	//Getter
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }
	DirectXCommon* GetDirectXCommon() { return dxCommon_; }
	
private:
	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);
private:
	DirectXCommon* dxCommon_ = nullptr;
	ComPtr<ID3D12RootSignature>rootSignature;
	ComPtr<ID3D12PipelineState>pipelineState;

	ComPtr<IDxcUtils> dxcUtils;
	ComPtr<IDxcCompiler3> dxcCompiler;
	ComPtr<IDxcIncludeHandler> includeHandler;

	ComPtr<ID3D10Blob>signatureBlob;
	ComPtr<ID3D10Blob>errorBlob;
};

