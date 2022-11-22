#include "..\..\Header\Graphics\SkinMeshAnimator.h"


GE::SkinMeshAnimator::SkinMeshAnimator() : SkinMeshAnimator(nullptr)
{
}

GE::SkinMeshAnimator::SkinMeshAnimator(SkinMeshData* setSkinMeshData)
	: skinMeshData(setSkinMeshData)
	, currentPlayAnimationData(nullptr)
	, isPlay(false)
	, isLoop(false)
	, isStartedFrame(false)
	, currentTime()
{
}

void GE::SkinMeshAnimator::PlayAnimation(const std::string& name, bool loopFlag)
{
	if (skinMeshData == nullptr)return;

	for (auto& playAnimation : skinMeshData->animationDatas)
	{
		if (playAnimation.name == name)
		{
			PlayAnimation(playAnimation.index, loopFlag);
			break;
		}
	}
}

void GE::SkinMeshAnimator::PlayAnimation(int index, bool loopFlag)
{
	if (index >= (int)skinMeshData->animationDatas.size())return;

	currentPlayAnimationData = &skinMeshData->animationDatas[index];

	skinMeshData->fbxScene->SetCurrentAnimationStack(currentPlayAnimationData->animStack);

	currentTime = 0;
	isPlay = true;
	isLoop = loopFlag;
	isStartedFrame = true;
}

void GE::SkinMeshAnimator::PlayAnimation()
{
	isPlay = true;
}

void GE::SkinMeshAnimator::ChangeAnimation(const std::string& name, int frameCount, bool loopFlag)
{
}

void GE::SkinMeshAnimator::StopAnimation()
{
	isPlay = false;
}

void GE::SkinMeshAnimator::Initialize()
{
	currentPlayAnimationData = nullptr;
	isPlay = false;
	isLoop = false;
	currentTime = 0;
}

void GE::SkinMeshAnimator::Update(float deltaTime)
{
	if (isPlay == false)return;

	// frameTimeの修正
	int fps = 1 / deltaTime;
	if (fps >= 60)
	{
		currentPlayAnimationData->frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
		if(fps >= 120)
		{
			currentPlayAnimationData->frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames120);
		}
	}

	// 開始し始めたフレーム時のデータが取得できるように
	if (isStartedFrame == true)
	{
		isStartedFrame = false;
		return;
	}

	if (currentTime >= currentPlayAnimationData->endTime && isLoop)
	{
		currentTime = 0;
		return;
	}

	// 時間更新
	currentTime += currentPlayAnimationData->frameTime;

	// このフレームで上限タイムを超えていた場合は値を修正
	if (currentTime >= currentPlayAnimationData->endTime)
	{
		currentTime = currentPlayAnimationData->endTime;
	}
}

void GE::SkinMeshAnimator::SetAnimationData(IGraphicsDeviceDx12* graphicsDevice, const Math::Matrix4x4& modelMatrix)
{
	ModelSkinMeshInfo skinMeshInfo;
	skinMeshInfo.modelMatrix = modelMatrix;

	// fbxMatrix から Math::Matrix4x4 に変換
	auto ConvertMatrixFromFbxMatrix = [](Math::Matrix4x4& dst, const FbxAMatrix& src)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				dst.m[i][j] = (float)src.Get(i, j);
			}
		}
	};

	for (int i = 0; i < (int)skinMeshData->bones.size(); ++i)
	{
		if (i >= MAX_BONES)break;

		Math::Matrix4x4 currentPose;
		FbxAMatrix fbxCurrentPose = skinMeshData->bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		ConvertMatrixFromFbxMatrix(currentPose, fbxCurrentPose);

		skinMeshInfo.bones[i] = skinMeshData->bones[i].invInitialPose * currentPose;
	}

	ShaderResourceCommand cbufferCommand;
	cbufferCommand.descIndex = 0;
	cbufferCommand.viewNumber = graphicsDevice->GetCBufferAllocater()->BindAndAttachData(0, &skinMeshInfo,sizeof(skinMeshInfo));
	graphicsDevice->GetRenderQueue()->AddSetConstantBufferInfo(cbufferCommand);
}
