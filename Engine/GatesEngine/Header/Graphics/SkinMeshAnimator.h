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

		// アニメーションの名前を検索し見つかればそのアニメーションを再生
		void PlayAnimation(const std::string& name, bool loopFlag = true);
		void PlayAnimation(int index,bool loopFlag = true);

		// stopAnimationをしたあとに続きからアニメーションさせたいときに使用
		void PlayAnimation();

		// アニメーションブレンド
		void ChangeAnimation(const std::string& name, int frameCount = 10,bool loopFlag = true);

		// isPlayをfalseにする
		void StopAnimation();

		void Initialize();
		void Update(float deltaTime);

		// 描画時に必要なアニメーションデータをシェーダーにバインド
		void SetAnimationData(IGraphicsDeviceDx12* graphicsDevice,const Math::Matrix4x4& modelMatrix);
	};
}