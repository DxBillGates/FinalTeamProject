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
	// �R���s���[�g�V�F�[�_�[�Ŏg���o�b�t�@�[�̐�������x�����s��
	void Initialize(GE::IGraphicsDeviceDx12* graphicsDevice);
	// �R���s���[�g�V�F�[�_�[�����s������
	void Compute();
	void Set3DTexture(int index,bool forCompute = true);
};