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
#include <GatesEngine/External/imgui/imgui.h>

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
	ScreenUIManager::GetInstance()->DashModeStart();
}

void DashModeScene::Update(float deltaTime)
{
	audioManager->Get("BGM2", 0)->Start();

	gameObjectManager.Update(deltaTime);
	collisionManager.Update();

	// LCtr + S �œG�̈ʒu�ۑ�
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::LCONTROL) &&
		inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::S))
	{
		EnemyManager::GetInstance()->SaveCurrentPosition("Resources/dashModeEnemies.txt");
		FieldObjectManager::GetInstance()->SaveCurrentPosition("Resources/tree.txt");
	}

	FieldObjectManager::GetInstance()->OtherUpdate();
	FieldObjectDebugTransform::GetInstance()->Update();
	ScreenUIManager::GetInstance()->DashModeUpdate(deltaTime);

	bool enemyDead = true;
	for (int i = 0; i < EnemyManager::GetInstance()->GetAllEnemies().size(); i++)
	{
		if (EnemyManager::GetInstance()->GetAllEnemies()[i]->GetComponent<Enemy>()->statas == Enemy::Statas::ALIVE)
		{
			enemyDead = false;
		}
	}

	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::OVER || enemyDead)
	{
		changeSceneInfo.name = "SampleScene";
		changeSceneInfo.flag = true;
		changeSceneInfo.initNextSceneFlag = true;
	}

	GE::Math::Matrix4x4 viewPort, proj, view;
	auto pos = ImGui::GetWindowPos();
	auto size = ImGui::GetWindowSize();
	GE::Math::Vector2 windowPos;
	GE::Math::Vector2 windowSize;
#ifdef _DEBUG
	windowPos = { pos.x,pos.y };
	windowSize = { size.x,size.y };
#else
	windowPos = { 0, 0 };
	windowSize = GE::Window::GetWindowSize();
#endif // _DEBUG

	const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();

	view = cameraInfo.viewMatrix;
	proj = cameraInfo.projMatrix;
	viewPort = GE::Math::Matrix4x4::GetViewportMatrix(windowSize, windowPos);
	GE::Math::Vector3 playerScreenPosition = GE::Math::Matrix4x4::Transform(gameObjectManager.FindGameObjectWithTag("Player", "player")->GetTransform()->position, view * proj);
	playerScreenPosition -= -1;
	playerScreenPosition /= 2;
	blurUV = { playerScreenPosition.x,playerScreenPosition.y };
	blurSampling = 16;
	blurThreshold = 0.5f;
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
	FieldObjectDebugTransform::GetInstance()->Draw();
	FieldObjectManager::GetInstance()->OtherDraw();

}

void DashModeScene::LateDraw()
{
	gameObjectManager.LateDraw();
	ScreenUIManager::GetInstance()->DrawSprite(graphicsDevice);
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
	PlayerComponent::dashMode = true;

	EnemyManager::GetInstance()->LoadPosition("Resources/dashModeEnemies.txt");
	EnemyManager::GetInstance()->Start(&gameObjectManager, true);
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

	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("miniMap", "miniMap"));
		auto* sampleComponent = testObject->AddComponent<MiniMapViewer>();
		sampleComponent->SetPlayer(gameObjectManager.FindGameObject("Player")->GetComponent<PlayerComponent>());
	}

}

void DashModeScene::UnLoad()
{

	EnemyManager::GetInstance()->UnLoad();
	FieldObjectManager::GetInstance()->UnLoad();
	FieldObjectDebugTransform::GetInstance()->UnLoad();
	UIObject::GetInstance()->UnLoad();
	PlayerColectObject::GetInstance()->ClearObject();
	ScreenUIManager::GetInstance()->UnLoad();
	// gameObjects���폜����
	Scene::UnLoad();


}