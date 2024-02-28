#include "Sprite.h"
#include<DirectXMath.h>
#include"BufferResource.h"
#include"Extemal/imgui/imgui.h"
#include "TextureManager.h"

using namespace Microsoft::WRL;
using namespace DirectX;
void Sprite::Initialize( SpriteCommon* common, std::wstring textureFilePath)
{
	common_ = common;
	dxCommon_ = common_->GetDirectXCommon();
	
	TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);

	//頂点情報
	CreateVertex();
	CreateIndex();
	//色
	CreateMaterial();
	//行列
	CreatWVP();
}
void Sprite::Update()
{
	transform.translate = { position.x,position.y ,0.0f};
	transform.rotate = { 0.0f,0.0f,rotation };
	transform.scale = { size.x,size.y,1.0f };

	materialData->color = color_;
	ImGui::Begin("Texture");
	ImGui::DragFloat3("Pos", &transform.translate.x, 0.1f);
	ImGui::DragFloat3("UV-Pos", &uvTransform.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UV-Rot", &uvTransform.rotate.z);
	ImGui::DragFloat3("UV-Scale", &uvTransform.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::End();
}
void Sprite::Draw()
{
	//transform.rotate.y += 0.03f;
	// ワールド
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.translate));
	// 回転行列とスケール行列の掛算
	XMMATRIX rotationAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);
	// 最終的な行列変換
	XMMATRIX worldMatrix = XMMatrixMultiply(rotationAndScaleMatrix, translationMatrix);
	
	// カメラ
	XMMATRIX cameraScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&cameraTransform.scale));
	XMMATRIX cameraRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&cameraTransform.rotate));
	XMMATRIX cameraTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&cameraTransform.translate));
	// 回転と行列の掛算
	XMMATRIX cameraRotationAndScaleMatrix = XMMatrixMultiply(cameraRotateMatrix, cameraScaleMatrix);
	// 最終的な行列
	XMMATRIX cameraWorldMatrix = XMMatrixMultiply(cameraRotationAndScaleMatrix, cameraTranslationMatrix);
	
	XMMATRIX view = XMMatrixInverse(nullptr, cameraWorldMatrix);
	XMMATRIX proj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		(float)WinApp::window_width / (float)WinApp::window_height,
		0.1f, 100.0f
	);

	XMMATRIX worldViewProjecionMatrix = worldMatrix * (view * proj);
	*wvpData = worldViewProjecionMatrix;

	XMMATRIX uvScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&uvTransform.scale));
	XMMATRIX uvRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&uvTransform.rotate));
	XMMATRIX uvTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&uvTransform.translate));
	XMMATRIX uvRotationAndScaleMatrix = XMMatrixMultiply(uvRotateMatrix, uvScaleMatrix);
	XMMATRIX uvWorldMatrix = XMMatrixMultiply(uvRotationAndScaleMatrix, uvTranslationMatrix);
	materialData->uvTransform = uvWorldMatrix;
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indeBufferView);
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0,materialResource->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex));
	//dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	dxCommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::CreateVertex()
{
	//VertexResource
	vertexResource = CreateBufferResouce(dxCommon_->GetDevice(), sizeof(VertexData) * 4);
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	//頂点情報
	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[1].position = { -0.5f,0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[3].position = { 0.5f,0.5f,0.0f,1.0f };
	vertexData[3].texcoord = { 1.0f,0.0f };
}

void Sprite::CreateIndex()
{
	indexResource = CreateBufferResouce(dxCommon_->GetDevice(),sizeof(uint32_t)*6);
	indeBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indeBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indeBufferView.Format = DXGI_FORMAT_R32_UINT;
	uint32_t* indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;
}

void Sprite::CreateMaterial()
{
	materialResource = CreateBufferResouce(dxCommon_->GetDevice(), sizeof(MaterialData));

	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	materialData->color = color_;
	materialData->uvTransform = XMMatrixIdentity();
}

void Sprite::CreatWVP()
{
	wvpResource = CreateBufferResouce(dxCommon_->GetDevice(), sizeof(XMMATRIX));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	*wvpData = XMMatrixIdentity();

}
