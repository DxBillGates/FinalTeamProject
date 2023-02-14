#include "NestScreenIndicator.h"
#include "PlayerComponent.h"

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
	renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("texture_Chick")->GetSRVNumber()});
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
}

void NestScreenIndicator::Update(float deltaTime)
{
	if (PlayerComponent::isChick == false)return;

	ScreenUI3DSpace::Update(deltaTime);

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
}

void NestScreenIndicator::SetAudioManager(GE::AudioManager* setManager)
{
	audioManager = setManager;
}
