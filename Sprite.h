#pragma once
#include"DirectXCommon.h"
#include<d3d12.h>
#include<wrl.h>
#include"SpriteCommon.h"
#include<DirectXMath.h>
//�P��
class Sprite
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	struct Transform {
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 rotate;
		DirectX::XMFLOAT3 translate;
	};

	struct VertexData {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texcoord;
	};

	struct MaterialData {
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX uvTransform;
	};
public:
	//������
	void Initialize( SpriteCommon* common, std::wstring textureFilePath);
	void Update();
	void Draw();
	DirectX::XMFLOAT2 GetPosition() { return position; }
	float GetRotation() { return rotation; }
	DirectX::XMFLOAT4 GetColor() { return color_; }
	void SetPosition(DirectX::XMFLOAT2 pos) { position = pos; }
	void SetRotation(float rot) { rotation = rot; }
	void SetColor(DirectX::XMFLOAT4 color) { color_ = color; }
	
	void SetSize(const DirectX::XMFLOAT2& size) { this->size = size; }
	const DirectX::XMFLOAT2& GetSize() { return size; }
private:
	//���_���
	void CreateVertex();
	void CreateIndex();
	//�}�e���A�����쐬
	void CreateMaterial();
	//�s����쐬
	void CreatWVP();
	uint32_t textureIndex = 0;
private:
	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* common_ = nullptr;
	ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	ComPtr<ID3D12Resource>indexResource;
	D3D12_INDEX_BUFFER_VIEW indeBufferView{};
	//�}�e���A�����
	ComPtr<ID3D12Resource>materialResource;
	MaterialData* materialData = nullptr;
	//�s����
	ComPtr<ID3D12Resource>wvpResource;
	DirectX::XMMATRIX* wvpData = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvhandleGPU;
	//�p�����[�^
	DirectX::XMFLOAT4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	Transform uvTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	DirectX::XMFLOAT2 position = { 0.0f,0.0f };
	float rotation = 0;
	Transform cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	DirectX::XMFLOAT2 size = { 1.0f,1.0f };

};

