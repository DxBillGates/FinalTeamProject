#include "VolumetricCloud.h"
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h>
#include <GatesEngine/Header/Graphics/COMRelease.h>

VolumetricCloud::VolumetricCloud()
{
}

VolumetricCloud::~VolumetricCloud()
{
	if (isInitialize == false)return;

	delete computePipeline;
	COM_RELEASE(updateNoiseTextureBuffer);
}

void VolumetricCloud::Initialize(GE::IGraphicsDeviceDx12* graphicsDevice)
{
	if (isInitialize)return;

	ID3D12Device* device = graphicsDevice->GetDevice();
	GE::IShaderResourceHeap* shaderResourceHeap = graphicsDevice->GetShaderResourceHeap();

	// テクスチャの解像度上げ過ぎるとメモリバカ食う64*64*64で約8mb、256*256*256で約512mb
	GE::Math::Vector3 resolution = { 32,32,32 };

	HRESULT resultHandle;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.CreationNodeMask = 1;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Alignment = 0;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc = { 1,0 };
	resDesc.Width = (UINT64)resolution.x;
	resDesc.Height = (UINT64)resolution.y;
	resDesc.DepthOrArraySize = (UINT64)resolution.z;

	resultHandle = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, IID_PPV_ARGS(&updateNoiseTextureBuffer));

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE3D;
	uavDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	uavDesc.Texture3D.MipSlice = 0;
	uavDesc.Texture3D.FirstWSlice = 0;
	uavDesc.Texture3D.WSize = (UINT64)resolution.z;
	usingUAVNumber = shaderResourceHeap->GetNextUAVNumber();
	shaderResourceHeap->CreateUAV(updateNoiseTextureBuffer, uavDesc);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture3D.MostDetailedMip = 0;
	srvDesc.Texture3D.MipLevels = 1;
	srvDesc.Texture3D.ResourceMinLODClamp = 0;
	usingSRVNumber = shaderResourceHeap->GetNextSRVNumber();
	shaderResourceHeap->CreateSRV(updateNoiseTextureBuffer, srvDesc);

	computePipeline = new GE::ComputePipeline(graphicsDevice,L"NoiseTextureGenerateComputeShader");
	computePipeline->Create({GE::DescriptorRangeType::UAV});

	this->graphicsDevice = graphicsDevice;
	isInitialize = true;
}

void VolumetricCloud::Compute()
{
	computePipeline->Set();
	graphicsDevice->SetShaderResourceDescriptorHeap();

	Set3DTexture(0);

	graphicsDevice->GetCmdList()->Dispatch(4, 4, 4);

	graphicsDevice->ExecuteCommands();
}

void VolumetricCloud::Set3DTexture(int index,bool forCompute)
{
	auto handle = graphicsDevice->GetShaderResourceHeap()->GetGPUHandleForUAV(usingUAVNumber);

	if (forCompute)graphicsDevice->GetCmdList()->SetComputeRootDescriptorTable(index, handle);
	else graphicsDevice->GetRenderQueue()->AddSetShaderResource({ index,usingSRVNumber });
}
