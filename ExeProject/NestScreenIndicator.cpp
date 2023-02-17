#include "NestScreenIndicator.h"
#include "PlayerComponent.h"
#include "CameraControl.h"
#include <GatesEngine/Header/GameFramework/GameSetting.h>
#include <GatesEngine/Header/Graphics/Window.h>
#include <GatesEngine/External/imgui/imgui.h>

bool NestScreenIndicator::isSetCameraDirection;

void NestScreenIndicator::SetShader()
{
	graphicsDevice->SetShader("DefaultSpriteWithTextureShader");
}

void NestScreenIndicator::SetShaderResource()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	graphicsDevice->SetCurrentRenderQueue(false);
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();
	GE::TextureAnimationInfo textureAnimationInfo;
	textureAnimationInfo.clipSize = { 1 };
	textureAnimationInfo.textureSize = { 1,1 };
	textureAnimationInfo.pivot = { 0 };
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4,&textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("texture_Chick")->GetSRVNumber() });
}

void NestScreenIndicator::Start()
{
	ScreenUI3DSpace::Start();

	waveFlagController.Initialize();
	waveFlagController.SetMaxTimeProperty(1);
	waveIntervalFlagContrller.Initialize();
	waveIntervalFlagContrller.SetFlag(true);
	waveIntervalFlagContrller.SetTime(2.9f);
	waveIntervalFlagContrller.SetMaxTimeProperty(3);
	returnCameraFlagController.Initialize();
	returnCameraFlagController.SetMaxTimeProperty(1);

	scale = { 50 };
	tempScale = scale;
	addScale = 0;

	player = gameObjectManager->FindGameObjectWithTag("Player", "player");
	nest = gameObjectManager->FindGameObjectWithTag("nest", "nest");

	isFirstSetCameraDirection = false;
	isSetCameraDirection = false;
}

void NestScreenIndicator::Update(float deltaTime)
{
	if (PlayerComponent::isChick == false)return;

	ScreenUI3DSpace::Update(deltaTime);

	bool isBeforeSetCameraDirection = isSetCameraDirection;
	if (PlayerComponent::isBeforeChick == false && PlayerComponent::isChick == true && isFirstSetCameraDirection == false)
	{
		isFirstSetCameraDirection = true;
		isSetCameraDirection = true;
	}


	// uiの拡縮用フラグ更新
	if (waveIntervalFlagContrller.GetOverTimeTrigger())
	{
		if (audioManager)audioManager->Use("hine1")->Start();
		waveFlagController.SetFlag(true);
		waveFlagController.SetTime(0);
		waveIntervalFlagContrller.SetTime(0);
		waveIntervalFlagContrller.SetFlag(false);
	}
	if (waveFlagController.GetOverTimeTrigger())
	{
		waveIntervalFlagContrller.SetFlag(true);
		waveIntervalFlagContrller.SetTime(0);
		waveFlagController.SetTime(0);
		waveFlagController.SetFlag(false);
		addScale = 0;
		isSetCameraDirection = false;
	}

	if (isSetCameraDirection)
	{
		CameraControl::GetInstance()->SetTargetObject(nest);
		GE::GameSetting::Time::SetGameTime(0.01f);

		// このときのカメラの情報を保存
		if (isBeforeSetCameraDirection == false)
		{
			const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
			beforeCameraDirection =
			{
				cameraInfo.cameraDir.x,
				cameraInfo.cameraDir.y,
				cameraInfo.cameraDir.z
			};
			beforeCameraPosition =
			{
				cameraInfo.cameraPos.x,
				cameraInfo.cameraPos.y,
				cameraInfo.cameraPos.z
			};
		}
	}

	if (isBeforeSetCameraDirection == true && isSetCameraDirection == false)
	{
		// ターゲット切り替え前にもう一度カメラの情報を保存
		const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
		currentCameraDirection =
		{
			cameraInfo.cameraDir.x,
			cameraInfo.cameraDir.y,
			cameraInfo.cameraDir.z
		};
		currentCameraPosition =
		{
			cameraInfo.cameraPos.x,
			cameraInfo.cameraPos.y,
			cameraInfo.cameraPos.z
		};
		returnCameraFlagController.SetTime(0);
		returnCameraFlagController.SetFlag(true);
		CameraControl::GetInstance()->SetTargetObject(player);
		GE::GameSetting::Time::SetGameTime(0.01f);
	}

	auto beforeReturnCameraFlag = returnCameraFlagController.GetFlag();

	if (returnCameraFlagController.GetFlag() == true)
	{
		auto camera = dynamic_cast<GE::Camera3DDebug*>(graphicsDevice->GetMainCamera());
		float returnElapsedTime = returnCameraFlagController.GetTime();
		if (returnElapsedTime > 1)returnElapsedTime = 1;
		float easingValue = GE::Math::Easing::EaseOutExpo(returnElapsedTime);
		camera->SetPosition(GE::Math::Vector3::Lerp(currentCameraPosition, player->GetTransform()->position - player->GetTransform()->GetForward() * 3000, easingValue));
		easingValue = GE::Math::Easing::EaseOutCirc(returnElapsedTime);
		camera->SetDirection(GE::Math::Vector3::Lerp(currentCameraDirection, player->GetTransform()->GetForward(), easingValue).Normalize());
		GE::GameSetting::Time::SetGameTime(0.01f);
	}

	if (returnCameraFlagController.GetOverTimeTrigger())
	{
		returnCameraFlagController.SetFlag(false);
		returnCameraFlagController.SetTime(0);
		GE::GameSetting::Time::SetGameTime(1);
	}

	addScale = GE::Math::Vector3(50) * std::sinf(waveFlagController.GetTime() * GE::Math::PI);
	scale += addScale;

	returnCameraFlagController.Update(deltaTime);
	waveIntervalFlagContrller.Update(deltaTime);
	waveFlagController.Update(deltaTime);
}

void NestScreenIndicator::Draw()
{
	if (PlayerComponent::isChick == false)return;

	scale = tempScale + addScale;
	ScreenUI3DSpace::Draw();

	// 矢印を描画
	if (isOffScreen == false)return;
	graphicsDevice->SetShader("DefaultSpriteWithTextureShader");
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	graphicsDevice->SetCurrentRenderQueue(false);
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();
	GE::TextureAnimationInfo textureAnimationInfo;
	textureAnimationInfo.clipSize = { 1 };
	textureAnimationInfo.textureSize = { 1,1 };
	textureAnimationInfo.pivot = { 0 };
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4,&textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("texture_indicatorArrow")->GetSRVNumber() });

	GE::Math::Matrix4x4 modelMatrix;
	GE::Math::Vector2 diffWindowSize = GE::Window::GetDiffWindowSize();
	GE::Math::Vector3 temp = scale / 2;
	temp.x *= diffWindowSize.x;
	temp.y *= diffWindowSize.y;
	modelMatrix = GE::Math::Matrix4x4::Scale(temp);
	GE::Math::Vector2 windowSize;
	windowSize = GE::Window::GetWindowSize();
	GE::Math::Vector3 windowSize3D = { windowSize.x,windowSize.y ,0 };
	GE::Math::Vector3 offscreenVector = calclatedScreenPos - windowSize3D / 2;
	temp = calclatedScreenPos + offscreenVector.Normalize() * (temp.x + 10);
	temp.x *= diffWindowSize.x;
	temp.y *= diffWindowSize.y;
	modelMatrix *= GE::Math::Matrix4x4::RotationZ(std::atan2f(offscreenVector.y, offscreenVector.x));
	modelMatrix *= GE::Math::Matrix4x4::Translate(temp);
	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(modelMatrix)) });
	DrawPlane();
}

void NestScreenIndicator::SetGameObjectManager(GE::GameObjectManager* setManager)
{
	gameObjectManager = setManager;
}

void NestScreenIndicator::SetAudioManager(GE::AudioManager* setManager)
{
	audioManager = setManager;
}

bool NestScreenIndicator::IsResetCameraFlag()
{
	return returnCameraFlagController.GetFlag() || isSetCameraDirection;
}
