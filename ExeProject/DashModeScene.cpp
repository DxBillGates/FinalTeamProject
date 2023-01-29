#include "DashModeScene.h"
#include "PlayerComponent.h"
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >

#include"MiniMapViewer.h"
#include"UIObject.h"

#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

#include"ScreenUI.h"
#include"FieldObjectDeBugTransform.h"
#include"FieldObjectManager.h"
#include"EnemyManager.h"
#include <GatesEngine/External/DirectXTex/DirectXTex.h>
#include "PlayerColectObject.h"

DashModeScene::DashModeScene()
	:DashModeScene("DashModeSceneScene")
{
}

DashModeScene::DashModeScene(const std::string& sceneName)
	: Scene(sceneName)
{
}

DashModeScene::DashModeScene(const std::string& sceneName, const GE::SceneInitializer& initializer)
	: Scene(sceneName, initializer)
{
}

DashModeScene::~DashModeScene()
{
}

void DashModeScene::Initialize()
{
	gameObjectManager.Awake();
	gameObjectManager.Start();
	directionalLight->GetAngle() = { 10,190 };

	PlayerComponent::dashMode = true;

}

void DashModeScene::Update(float deltaTime)
{
	gameObjectManager.Update(deltaTime);
	collisionManager.Update();

	// LCtr + S ‚Å“G‚ÌˆÊ’u•Û‘¶
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::LCONTROL) &&
		inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::S))
	{
		EnemyManager::GetInstance()->SaveCurrentPosition("Resources/dashModeEnemies.txt");
		FieldObjectManager::GetInstance()->SaveCurrentPosition("Resources/tree.txt");
	}

	FieldObjectManager::GetInstance()->OtherUpdate();
	FieldObjectDebugTransform::GetInstance()->Update();

	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::OVER)
	{
		changeSceneInfo.name = "SampleScene";
		changeSceneInfo.flag = true;
		changeSceneInfo.initNextSceneFlag = true;
	}
}

void DashModeScene::Draw()
{
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::Camera* mainCamera = graphicsDevice->GetMainCamera();

	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = directionalLight->GetViewMatrix();
	cameraInfo.projMatrix = directionalLight->GetProjectionMatrix();
	cameraInfo.lightMatrix = directionalLight->GetVPMatrix();

	graphicsDevice->ClearLayer("shadowLayer");
	graphicsDevice->SetLayer("shadowLayer");
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	directionalLight->SetDirectionalLightInfo();
	gameObjectManager.DrawShadow();

	graphicsDevice->ExecuteRenderQueue();
	graphicsDevice->ExecuteCommands();

	graphicsDevice->SetShaderResourceDescriptorHeap();
	cameraInfo = mainCamera->GetCameraInfo();
	cameraInfo.lightMatrix = directionalLight->GetVPMatrix();
	graphicsDevice->SetLayer("resultLayer");
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	directionalLight->SetDirectionalLightInfo();

	gameObjectManager.Draw();
	FieldObjectManager::GetInstance()->OtherDraw();
}

void DashModeScene::LateDraw()
{
	gameObjectManager.LateDraw();
}

void DashModeScene::Load()
{
	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("Player", "player"));
		auto* sampleComponent = testObject->AddComponent<PlayerComponent>();
		testObject->GetComponent<PlayerComponent>()->SetAudioManager(audioManager);
		auto* playerCollider = testObject->AddComponent < GE::SphereCollider >();
		playerCollider->SetCenter({ 0,0,0 });
		playerCollider->SetSize({ 20 });
	}
	EnemyManager::GetInstance()->LoadPosition("Resources/dashModeEnemies.txt");
	EnemyManager::GetInstance()->Start(&gameObjectManager);
	FieldObjectManager::GetInstance()->LoadPosition("Resources/tree.txt");
	FieldObjectManager::GetInstance()->Start(&gameObjectManager);
	collisionManager.AddTagCombination("player", "enemy");
	collisionManager.AddTagCombination("player", "frog");
	collisionManager.AddTagCombination("player", "ground");
	collisionManager.AddTagCombination("player", "StartTree");
	collisionManager.AddTagCombination("player", "nest");
	collisionManager.AddTagCombination("player", "tile");
	collisionManager.AddTagCombination("player", "fieldtree");
	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("directionalLight", "directionalLight"));
		directionalLight = testObject->AddComponent<GE::DirectionalLight>();
		//directionalLight->SetTarget(gameObjectManager.FindGameObjectWithTag("Player", "player")->GetTransform());
		testObject->GetTransform()->position = { 0,0,0 };
	}

}

void DashModeScene::UnLoad()
{
	EnemyManager::GetInstance()->UnLoad();
	FieldObjectManager::GetInstance()->UnLoad();
	// gameObjects‚ğíœ‚·‚é
	Scene::UnLoad();


}