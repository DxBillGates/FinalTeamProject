#pragma once
#include "..\..\Header\Graphics\SkinMeshManager.h"
#include "IGraphicsDeviceDx12.h"

namespace GE
{
	class SkinMeshAnimator
	{
	private:
		SkinMeshData* skinMeshData;
		SkinMeshAnimationData* currentPlayAnimationData;
		bool isPlay;
		bool isLoop;
		bool isStartedFrame;

		FbxTime currentTime;
	public:
		SkinMeshAnimator();
		SkinMeshAnimator(SkinMeshData* setSkinMeshData);

		// �A�j���[�V�����̖��O��������������΂��̃A�j���[�V�������Đ�
		void PlayAnimation(const std::string& name, bool loopFlag = true);
		void PlayAnimation(int index,bool loopFlag = true);

		// stopAnimation���������Ƃɑ�������A�j���[�V�������������Ƃ��Ɏg�p
		void PlayAnimation();

		// �A�j���[�V�����u�����h
		void ChangeAnimation(const std::string& name, int frameCount = 10,bool loopFlag = true);

		// isPlay��false�ɂ���
		void StopAnimation();

		void Initialize();
		void Update(float deltaTime);

		// �`�掞�ɕK�v�ȃA�j���[�V�����f�[�^���V�F�[�_�[�Ƀo�C���h
		void SetAnimationData(IGraphicsDeviceDx12* graphicsDevice,const Math::Matrix4x4& modelMatrix);
	};
}