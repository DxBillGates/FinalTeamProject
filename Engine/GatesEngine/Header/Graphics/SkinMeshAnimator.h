#pragma once
#include "..\..\Header\Graphics\SkinMeshManager.h"
#include "IGraphicsDeviceDx12.h"

namespace GE
{
	class SkinMeshAnimator
	{
	private:
		SkinMeshData* animationData;
		SkinMeshAnimationData* currentPlayAnimationData;
		bool isPlay;
		bool isLoop;
	public:
		SkinMeshAnimator();
		SkinMeshAnimator(SkinMeshAnimationData* setAnimationData);

		// �A�j���[�V�����̖��O��������������΂��̃A�j���[�V�������Đ�
		void PlayAnimation(const std::string& name,bool loopFlag);
		void PlayAnimation(int index,bool loopFlag);

		// stopAnimation���������Ƃɑ�������A�j���[�V�������������Ƃ��Ɏg�p
		void PlayAnimation();

		// isPlay��false�ɂ���
		void StopAnimation();

		void Initialize();
		void Update();

		// �`�掞�ɕK�v�ȃA�j���[�V�����f�[�^���V�F�[�_�[�Ƀo�C���h
		void SetAnimationData(IGraphicsDeviceDx12* graphicsDevice);
	};
}