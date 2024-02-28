#include "Sprite.h"
#include<DirectXMath.h>
#include"BufferResource.h"
using namespace Microsoft::WRL;
using namespace DirectX;
void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common)
{
	dxCommon_ = dxCommon;
	common_ = common;
	DirectX::ScratchImage mipImages = common->LoadTexture(L"Resources/mario.jpg");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(dxCommon_->GetDevice(), metaData);
	common_->UploadTextureData(textureResource, mipImages);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metaData.mipLevels);
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvhandleCPU = dxCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvhandleGPU = dxCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	textureSrvhandleCPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvhandleGPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvhandleCPU);
	//頂点情報
	CreateVertex();
	//色
	CreateMaterial();
	//行列
	CreatWVP();
}
void Sprite::Draw()
{
	transform.rotate.y += 0.03f;
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.translate));
	XMMATRIX rotationAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);
	XMMATRIX worldMatrix = XMMatrixMultiply(rotationAndScaleMatrix, translationMatrix);
	XMMATRIX cameraScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&cameraTransform.scale));
	XMMATRIX cameraRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&cameraTransform.rotate));
	XMMATRIX cameraTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&cameraTransform.translate));
	XMMATRIX cameraRotationAndScaleMatrix = XMMatrixMultiply(cameraRotateMatrix, cameraScaleMatrix);
	XMMATRIX cameraWorldMatrix = XMMatrixMultiply(cameraRotationAndScaleMatrix, cameraTranslationMatrix);
	XMMATRIX view = XMMatrixInverse(nullptr, scaleMatrix);
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)WinApp::window_width / (float)WinApp::window_height, 0.1f, 100.0f);
	XMMATRIX worldViewProjecionMatrix = worldMatrix * (view * proj);
	*wvpData = worldMatrix;
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxCommon_->GetCommandList()->SetPipelineState(common_->GetPipelineState());
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0,materialResource->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void Sprite::CreateVertex()
{
	//VertexResource
	vertexResource = CreateBufferResouce(dxCommon_->GetDevice(), sizeof(VertexData) * 3);
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 3;
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	//頂点情報
	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.5f,0.0f };
	vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };
}

void Sprite::CreateMaterial()
{
	materialResource = CreateBufferResouce(dxCommon_->GetDevice(), sizeof(XMFLOAT4));

	XMFLOAT4* materialData = nullptr;
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	*materialData = color_;
}

void Sprite::CreatWVP()
{
	wvpResource = CreateBufferResouce(dxCommon_->GetDevice(), sizeof(XMMATRIX));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	*wvpData = XMMatrixIdentity();
}
