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

	// UIをランダムな間隔で一定時間拡縮させる際にしよう
	GE::FlagController waveFlagController;
	GE::FlagController waveIntervalFlagContrller;
	GE::AudioManager* audioManager;
	GE::GameObjectManager* gameObjectManager;

	GE::GameObject* player;
	GE::GameObject* nest;

	bool isFirstSetCameraDirection;

	GE::FlagController returnCameraFlagController;
	GE::Math::Vector3 beforeCameraPosition;
	GE::Math::Vector3 beforeCameraDirection;
	GE::Math::Vector3 currentCameraPosition;
	GE::Math::Vector3 currentCameraDirection;
private:
	void SetShader()override;
	void SetShaderResource()override;
public:
	void Start() override;
	void Update(float deltaTime)override;
	void Draw() override;
	void SetGameObjectManager(GE::GameObjectManager* setManager);
	void SetAudioManager(GE::AudioManager* setManager);
	bool IsResetCameraFlag();
};
