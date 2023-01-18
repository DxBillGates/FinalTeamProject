#pragma once
#include <GatesEngine/Header/Graphics/ComputePipeline.h>

class VolumetricCloud
{
private:
	bool isInitialize;
	// uav using compute shader for update
	ID3D12Resource* updateNoiseTextureBuffer;

	int usingUAVNumber;
	int usingSRVNumber;

	GE::IGraphicsDeviceDx12* graphicsDevice;
	GE::ComputePipeline* computePipeline;
public:
	VolumetricCloud();
	~VolumetricCloud();
	// コンピュートシェーダーで使うバッファーの生成を一度だけ行う
	void Initialize(GE::IGraphicsDeviceDx12* graphicsDevice);
	// コンピュートシェーダーを実行させる
	void Compute();
	void Set3DTexture(int index,bool forCompute = true);
};