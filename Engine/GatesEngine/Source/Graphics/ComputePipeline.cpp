#include "..\..\Header\Graphics\ComputePipeline.h"
#include "..\..\Header\Graphics\COMRelease.h"
#include <d3dcompiler.h>

GE::ComputePipeline::ComputePipeline(IGraphicsDeviceDx12* pGraphicsDevice, const std::wstring& filename)
	: graphicsDevice(pGraphicsDevice)
	, shaderBlob(nullptr)
	, pipeline(nullptr)
	, rootSignature()
{
	//CSのコンパイル＆生成
	std::wstring filepath = L"Resources/Shader/" + filename + L".hlsl";
	auto hr = D3DCompileFromFile(filepath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shaderBlob, nullptr);
}

GE::ComputePipeline::~ComputePipeline()
{
	COM_RELEASE(shaderBlob);
	COM_RELEASE(pipeline);
}

void GE::ComputePipeline::Create(const std::vector<GE::DescriptorRangeType>& range)
{
	//ルートシグネチャの生成
	rootSignature.Create(graphicsDevice->GetDevice(), range);

	D3D12_COMPUTE_PIPELINE_STATE_DESC pipeDesc = {};
	pipeDesc.CS.pShaderBytecode = shaderBlob->GetBufferPointer();
	pipeDesc.CS.BytecodeLength = shaderBlob->GetBufferSize();
	pipeDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	pipeDesc.NodeMask = 0;
	pipeDesc.pRootSignature = rootSignature.GetRootSignature();

	HRESULT hr = graphicsDevice->GetDevice()->CreateComputePipelineState(&pipeDesc, IID_PPV_ARGS(&pipeline));
}

void GE::ComputePipeline::Set()
{
	graphicsDevice->GetCmdList()->SetComputeRootSignature(rootSignature.GetRootSignature());
	graphicsDevice->GetCmdList()->SetPipelineState(pipeline);
}
