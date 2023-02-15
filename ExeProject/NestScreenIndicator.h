#pragma once
#include "ScreenUI3DSpace.h"
#include <GatesEngine/Header/GameFramework/FlagController.h>
#include <GatesEngine/Header/Audio/AudioManager.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h>

class NestScreenIndicator : public ScreenUI3DSpace
{
public:
	static bool isSetCameraDirection;
private:
	GE::Math::Vector3 addScale;
	GE::Math::Vector3 tempScale;

	// UI‚ğƒ‰ƒ“ƒ_ƒ€‚ÈŠÔŠu‚Åˆê’èŠÔŠgk‚³‚¹‚éÛ‚É‚µ‚æ‚¤
	GE::FlagController waveFlagController;
	GE::FlagController waveIntervalFlagContrller;
	GE::AudioManager* audioManager;
	GE::GameObjectManager* gameObjectManager;

	GE::GameObject* player;
	GE::GameObject* nest;

	bool isFirstSetCameraDirection;
private:
	void SetShader()override;
	void SetShaderResource()override;
public:
	void Start() override;
	void Update(float deltaTime)override;
	void Draw() override;
	void SetGameObjectManager(GE::GameObjectManager* setManager);
	void SetAudioManager(GE::AudioManager* setManager);
};
