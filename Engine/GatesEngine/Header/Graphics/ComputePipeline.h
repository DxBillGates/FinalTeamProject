#pragma once
#include "IGraphicsDeviceDx12.h"
#include "RootSignature.h"
#include <d3d12.h>
#include <string>
#include <vector>

namespace GE
{
	/// <summary>
	/// �R���s���[�g�V�F�[�_��������悤�ɂ���֐�
	/// </summary>
	class ComputePipeline
	{
	private:
		IGraphicsDeviceDx12* graphicsDevice;
		ID3DBlob* shaderBlob;
		ID3D12PipelineState* pipeline;
		RootSignature rootSignature;
	public:
		/// <summary>
		/// �R���X�g���N�^�A���V�F�[�_�[�R���p�C��
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDevice�̃|�C���^</param>
		/// <param name="filename">�R���p�C������V�F�[�_�[�t�@�C���̖��O</param>
		ComputePipeline(IGraphicsDeviceDx12* pGraphicsDevice, const std::wstring& filename);
		~ComputePipeline();

		/// <summary>
		/// �����֐�
		/// </summary>
		/// <param name="range">�V�F�[�_�[�ň���RootParam�̔z��</param>
		void Create(const std::vector<GE::DescriptorRangeType>& range);

		/// <summary>
		/// �V�F�[�_�[���Z�b�g����֐�
		/// </summary>
		void Set();
	};
}
