#pragma once
#include<d3d12.h>
#include<cassert>

ID3D12Resource* CreateBufferResouce(ID3D12Device* device, size_t sizeInByte) {
	ID3D12Resource* resource = nullptr;
	//VertexResousource
	D3D12_HEAP_PROPERTIES uploadHeapPropertties{};
	uploadHeapPropertties.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC ResourceDesc{};
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInByte;
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT result = device->CreateCommittedResource(&uploadHeapPropertties, D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));
	return resource;
}