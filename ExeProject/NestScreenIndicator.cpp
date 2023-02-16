#include "NestScreenIndicator.h"
#include "PlayerComponent.h"
#include "CameraControl.h"
#include <GatesEngine/Header/GameFramework/GameSetting.h>
#include <GatesEngine/Header/Graphics/Window.h>

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

	if (PlayerComponent::isBeforeChick == false && PlayerComponent::isChick == true && isFirstSetCameraDirection == false)
	{
		isFirstSetCameraDirection = true;
		isSetCameraDirection = true;
	}

	bool isBeforeSetCameraDirection = isSetCameraDirection;

	// ui‚ÌŠgk—pƒtƒ‰ƒOXV
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
	}

	if (isBeforeSetCameraDirection == true && isSetCameraDirection == false)
	{
		CameraControl::GetInstance()->SetTargetObject(player);
		GE::GameSetting::Time::SetGameTime(1.0f);
	}

	addScale = GE::Math::Vector3(50) * std::sinf(waveFlagController.GetTime() * GE::Math::PI);
	scale += addScale;

	waveIntervalFlagContrller.Update(deltaTime);
	waveFlagController.Update(deltaTime);
}

void NestScreenIndicator::Draw()
{
	if (PlayerComponent::isChick == false)return;

	scale = tempScale + addScale;
	ScreenUI3DSpace::Draw();

	// –îˆó‚ð•`‰æ
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
	modelMatrix *= GE::Math::Matrix4x4::RotationZ(std::atan2f(offscreenVector.y,offscreenVector.x));
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
