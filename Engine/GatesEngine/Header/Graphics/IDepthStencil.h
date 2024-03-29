#pragma once
#include <d3d12.h>

namespace GE
{
	class IDepthStencil
	{
	public:
		virtual ~IDepthStencil() {}
		virtual D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() = 0;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() = 0;
	};
}