#pragma once
#include "..\..\Audio\AudioManager.h"
#include "..\..\Input\InputDevice.h"
#include "..\..\Graphics\IGraphicsDeviceDx12.h"
#include "..\..\GameFramework\GameObject\GameObjectManager.h"
#include "..\..\GameFramework\Collision\CollisionManager.h"
#include "..\FlagController.h"

#include <string>

namespace GE
{
	struct SceneInitializer
	{
		AudioManager* audioManager;
		InputDevice* inputDevice;
		IGraphicsDeviceDx12* graphicsDevice;

		SceneInitializer()
			: audioManager(nullptr)
			, inputDevice(nullptr)
			, graphicsDevice(nullptr)
		{
		}

		SceneInitializer(AudioManager* audioManager, InputDevice* inputDevice,IGraphicsDeviceDx12* grpahicsDevice)
			: audioManager(audioManager)
			, inputDevice(inputDevice)
			, graphicsDevice(grpahicsDevice)
		{
		}
	};

	struct ChangeSceneInfo
	{
		std::string name;
		bool flag;
		bool initNextSceneFlag;
		FlagController sceneTransitionFadeout;
		FlagController sceneTransitionFadein;

		ChangeSceneInfo()
			: name({})
			, flag(false)
			, initNextSceneFlag(false)
			, sceneTransitionFadeout()
			, sceneTransitionFadein()
		{
			sceneTransitionFadein.SetMaxTimeProperty(1);
			sceneTransitionFadein.SetFlag(true);
		}
	};

	class Scene
	{
	protected:
		std::string name;
		bool isTerminateApplication;

		bool isSetInitializer;
		ChangeSceneInfo changeSceneInfo;
		AudioManager* audioManager;
		InputDevice* inputDevice;
		IGraphicsDeviceDx12* graphicsDevice;
		GameObjectManager gameObjectManager;
		CollisionManager collisionManager;

		float blurSampling;
		Math::Vector2 blurUV;
		float blurThreshold;
	public:
		Scene();
		Scene(const std::string& sceneName);
		Scene(const std::string& sceneName,const SceneInitializer& initializer);
		virtual ~Scene() {}
		virtual void Initialize() {}
		virtual void Update(float deltaTime){}
		virtual void Draw() {}
		virtual void LateDraw() {}
		virtual void Load() {}
		virtual void UnLoad();

		const std::string& GetSceneName();
		bool IsTerminateApplication();
		ChangeSceneInfo& IsChangeScene();

		void SetSceneInitializer(const SceneInitializer& initializer);
		float GetBlurSampling();
		Math::Vector2 GetBlurUV();
		float GetBlurThreshold();
	};
}
