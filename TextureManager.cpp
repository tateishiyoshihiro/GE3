#include "TextureManager.h"
#include "BufferResource.h"

TextureManager* TextureManager::instance = nullptr;
uint32_t TextureManager::kSRVIndexTop = 1;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void TextureManager::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;
	textureDatas.reserve(DirectXCommon::kMaxSRVCount);
}

void TextureManager::LoadTexture(const std::wstring& filePath)
{
	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {
			return textureData.filePath == filePath;
	});

	if (it != textureDatas.end()) {
		return;
	}

	assert(textureDatas.size() + kSRVIndexTop < DirectXCommon::kMaxSRVCount);

	DirectX::ScratchImage image{};
	HRESULT result = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(result));

	DirectX::ScratchImage mipImage{};
	result = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImage);
	assert(SUCCEEDED(result));
	textureDatas.resize(textureDatas.size() + 1);
	TextureData& textureData = textureDatas.back();
	textureData.filePath = filePath;
	textureData.metadata = mipImage.GetMetadata();
	textureData.resource = CreateTextureResource(dxCommon_->GetDevice(),textureData.metadata);
	UploadTextureData(textureData.resource.Get(), mipImage);
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() -1) + kSRVIndexTop;
	textureData.srvHandleCPU = dxCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureData.srvHandleGPU = dxCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	textureData.srvHandleCPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureData.srvHandleGPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(textureData.metadata.mipLevels);
	dxCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);

}

void TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	for (size_t mipLevel = 0; mipLevel < metaData.mipLevels; ++mipLevel) {
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		HRESULT result = texture->WriteToSubresource(
			UINT(mipLevel), nullptr,
			img->pixels, UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(result));
	}
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(uint32_t textureIndex)
{
	assert(dxCommon_->kMaxSRVCount > textureIndex);
	TextureData& textureData = textureDatas[textureIndex];
	return textureData.srvHandleGPU;
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::wstring& filePath)
{
	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {
			return textureData.filePath == filePath;
		});
	if (it != textureDatas.end()) {
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}
	assert(0);
	return 0;
}
