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

		// アニメーションの名前を検索し見つかればそのアニメーションを再生
		void PlayAnimation(const std::string& name,bool loopFlag);
		void PlayAnimation(int index,bool loopFlag);

		// stopAnimationをしたあとに続きからアニメーションさせたいときに使用
		void PlayAnimation();

		// isPlayをfalseにする
		void StopAnimation();

		void Initialize();
		void Update();

		// 描画時に必要なアニメーションデータをシェーダーにバインド
		void SetAnimationData(IGraphicsDeviceDx12* graphicsDevice);
	};
}