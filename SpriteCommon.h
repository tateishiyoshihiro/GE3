#pragma once
#include<wrl.h>
#include <string>
#include<dxcapi.h>
#include <d3d12.h>
#include"DirectXCommon.h"
#include <../DirectXTex.h>

//ã§í 
class SpriteCommon
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//èâä˙âª
	void Initialize(DirectXCommon*dxCommon);
	//Getter
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }
	//âÊëúÇÃì«Ç›çûÇ›
	DirectX::ScratchImage LoadTexture(const std::wstring& filepath);
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
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

