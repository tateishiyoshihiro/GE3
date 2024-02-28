#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include<vector>
#include <chrono>

#include "WinApp.h"
//DirectX基盤
class DirectXCommon
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバー関数
	///初期化
	void Initialize(WinApp* winApp);
	void PreDraw();
	void Postdraw();
	ID3D12Device* GetDevice()const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList.Get(); }
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc; }
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() { return rtvDesc; }
	ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return srvDescriptorHeap.Get(); }
	static const uint32_t kMaxSRVCount;
private:
	//デバイス
	void DeviceInitialize();
	//コマンド
	void CommandInitialize();
	//スワップチェーン
	void SwapChainInitialize();
	//レンダーターゲット
	void RenderTargetInitialize();
	//深度バッファ
	void DepthBufferInitialize();
	//フェンス
	void FenceInitialize();
	//FPS固定初期化
	void InitializeFixFPS();
	//FPS固定更新
	void UpdateFixFPS();
	//ディスクリプタヒープ作成
	ID3D12DescriptorHeap* CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType,UINT numDescripots,bool shaderVisible);

private:
	WinApp* winApp = nullptr;

	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;

	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	ComPtr<IDXGISwapChain4> swapChain;

	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	ComPtr<ID3D12Resource> depthBuff;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	D3D12_RESOURCE_BARRIER barrierDesc{};
	//
	std::chrono::steady_clock::time_point reference_;
	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//ディスクリプタヒープ
	//RTV(ゲーム画面の保存)
	ComPtr<ID3D12DescriptorHeap>rtvDescriptorHeap;
	//SRV(画像の保存)
	ComPtr<ID3D12DescriptorHeap>srvDescriptorHeap;


};

