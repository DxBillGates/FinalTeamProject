#include "..\..\..\Header\GameFramework\Scene\Scene.h"

GE::Scene::Scene() 
	: Scene("unregister name scene")
{
}

GE::Scene::Scene(const std::string& sceneName)
	: name(sceneName)
	, isTerminateApplication(false)
	, isSetInitializer(false)
	, audioManager(nullptr)
	, inputDevice(nullptr)
	, graphicsDevice(nullptr)
	, gameObjectManager(GameObjectManager())
	, collisionManager(CollisionManager())
	, blurSampling(8)
	, blurUV({0.5f,0.5f})
	, blurThreshold(0)
{
	collisionManager.SetGameObjectManager(gameObjectManager.GetManager());
}

GE::Scene::Scene(const std::string& sceneName, const SceneInitializer& initializer)
	: Scene(sceneName)
{
	SetSceneInitializer(initializer);
}

void GE::Scene::UnLoad()
{
	gameObjectManager.DestroyGameObjects();
}


const std::string& GE::Scene::GetSceneName()
{
	return name;
}

bool GE::Scene::IsTerminateApplication()
{
	return isTerminateApplication;
}

GE::ChangeSceneInfo& GE::Scene::IsChangeScene()
{
	return changeSceneInfo;
}

void GE::Scene::SetSceneInitializer(const SceneInitializer& initializer)
{
	audioManager = initializer.audioManager;
	inputDevice = initializer.inputDevice;
	graphicsDevice = initializer.graphicsDevice;
	isSetInitializer = true;
}

float GE::Scene::GetBlurSampling()
{
	return blurSampling;
}

GE::Math::Vector2 GE::Scene::GetBlurUV()
{
	return blurUV;
}

float GE::Scene::GetBlurThreshold()
{
	return blurThreshold;
}
