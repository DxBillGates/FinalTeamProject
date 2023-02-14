#pragma once
#include "ScreenUI3DSpace.h"
#include <GatesEngine/Header/GameFramework/FlagController.h>
#include <GatesEngine/Header/Audio/AudioManager.h>

class NestScreenIndicator : public ScreenUI3DSpace
{
private:
	GE::Math::Vector3 addScale;
	GE::Math::Vector3 tempScale;

	// UI‚ğƒ‰ƒ“ƒ_ƒ€‚ÈŠÔŠu‚Åˆê’èŠÔŠgk‚³‚¹‚éÛ‚É‚µ‚æ‚¤
	GE::FlagController waveFlagController;
	GE::FlagController waveIntervalFlagContrller;
	GE::AudioManager* audioManager;
private:
	void SetShader()override;
	void SetShaderResource()override;
public:
	void Start() override;
	void Update(float deltaTime)override;
	void Draw() override;
	void SetAudioManager(GE::AudioManager* setManager);
};
