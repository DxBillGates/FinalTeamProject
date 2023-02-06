#include "SampleScene.h"
#include "PlayerComponent.h"
#include"EnemyManager.h"
#include"FieldObjectManager.h"
#include <GatesEngine/Header\GameFramework\Component\SampleComponent.h>
#include <GatesEngine/Header\GameFramework\Component\SphereCollider.h>
#include <GatesEngine/Header\GameFramework\Component\BoxCollider.h>
#include <GatesEngine/Header\GameFramework\Collision\CollisionManager.h>
#include <GatesEngine/Header/Application/Application.h>
#include"Title.h"
#include"StartTree.h"
#include"TimeLimit.h"
#include"MiniMapViewer.h"
#include"Collect.h"
#include"UIObject.h"

#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

#include"ScreenUI.h"
#include"FieldObjectDeBugTransform.h"
#include <GatesEngine/External/DirectXTex/DirectXTex.h>
#include "VolumetricCloud.h"
#include "PlayerColectObject.h"

SampleScene::SampleScene()
	: SampleScene("SampleScene")
{
}

SampleScene::SampleScene(const std::string& sceneName)
	: SampleScene(sceneName, {})
{
}

SampleScene::SampleScene(const std::string& sceneName, const GE::SceneInitializer& initializer)
	: Scene(sceneName, initializer)
	, col1(nullptr)
	, col2(nullptr)
{

	auto fieldObjectManager = FieldObjectManager::GetInstance();
	fieldObjectManager->SetGraphicsDevice(graphicsDevice);
	fieldObjectManager->LoadModels();
}

SampleScene::~SampleScene()
{
}

void SampleScene::Initialize()
{
	Title::GetInstance()->Awake(&gameObjectManager, graphicsDevice);

	gameObjectManager.Awake();
	gameObjectManager.Start();
	UIObject::GetInstance()->SampleSceneStart();
	ScreenUIManager::GetInstance()->SampleSceneStart();
	ScreenUIManager::GetInstance()->SetAudioManager(audioManager);

	changeSceneInfo.sceneTransitionFadein.SetMaxTimeProperty(2);
	changeSceneInfo.sceneTransitionFadeout.SetMaxTimeProperty(0.2f);
}

void SampleScene::Update(float deltaTime)
{
	audioManager->Get("testBGM", 0)->Start();


	GE::Math::Vector3 directionalLightAngle = { directionalLight->GetAngle().x,directionalLight->GetAngle().y,0 };
	directionalLightAngle.x += GE::GameSetting::Time::GetDeltaTime();
	GE::Math::Vector3 minAngle = { 10,directionalLightAngle.y,0 };
	GE::Math::Vector3 maxAngle = { 170,directionalLightAngle.y,0 };
	directionalLightAngle = GE::Math::Vector3::Min(minAngle, GE::Math::Vector3::Max(maxAngle, directionalLightAngle));
	directionalLight->GetAngle().x = directionalLightAngle.x;

	Title::GetInstance()->Select();
	gameObjectManager.Update(deltaTime);
	collisionManager.Update();
	Title::GetInstance()->Update(audioManager);
	TimeLimit::GetInstance()->Update(audioManager);
	//Collect::GetInstance()->Update(StartTree::collectCount, StartTree::goalCollect);
	UIObject::GetInstance()->SampleSceneUpdate(deltaTime);
	ScreenUIManager::GetInstance()->SampleSceneUpdate(deltaTime);
	//ƒNƒŠƒAˆÈ~ğŒ
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D1)
		|| StartTree::isCollect)
	{
		changeSceneInfo.flag = true;
		changeSceneInfo.name = "ClearScene";
		changeSceneInfo.initNextSceneFlag = true;
	}
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D2)
		|| TimeLimit::GetInstance()->GetTimeOver())
	{
		changeSceneInfo.flag = true;
		changeSceneInfo.name = "OverScene";
		changeSceneInfo.initNextSceneFlag = true;
	}

	//Collect::GetInstance()->Update(StartTree::collectCount, StartTree::goalCollect);
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::Y))
	{
		changeSceneInfo.flag = true;
		changeSceneInfo.name = "SampleScene";
		changeSceneInfo.initNextSceneFlag = true;
	}

	if (Title::GetInstance()->GetSelect(Title::States::endless))
	{
		changeSceneInfo.flag = true;
		changeSceneInfo.name = "DashModeScene";
		changeSceneInfo.initNextSceneFlag = true;
	}

	// LCtr + S ‚Å“G‚ÌˆÊ’u•Û‘¶
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::LCONTROL) &&
		inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::S))
	{
		EnemyManager::GetInstance()->SaveCurrentPosition("Resources/enemies.txt");
		FieldObjectManager::GetInstance()->SaveCurrentPosition("Resources/tree.txt");
	}

	FieldObjectManager::GetInstance()->OtherUpdate();
	FieldObjectDebugTransform::GetInstance()->Update();

	blurThreshold = GE::Math::Lerp(0, 0.5f, (PlayerComponent::current_speed - 20) / 100.0f);
}

void SampleScene::Draw()
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

	DirectX::XMMATRIX t = DirectX::XMMatrixPerspectiveFovLH(90, 1920 / 1080, 1, 60000);
	cameraInfo.lightMatrix = directionalLight->GetVPMatrix();
	graphicsDevice->SetLayer("resultLayer");
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	directionalLight->SetDirectionalLightInfo();
	gameObjectManager.Draw();
	FieldObjectManager::GetInstance()->OtherDraw();
	FieldObjectDebugTransform::GetInstance()->Draw();
	UIObject::GetInstance()->Draw(graphicsDevice);
}

void SampleScene::LateDraw()
{
	gameObjectManager.LateDraw();
	ScreenUIManager::GetInstance()->DrawSprite(graphicsDevice);
}

void SampleScene::Load()
{
	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("Player", "player"));
		auto* sampleComponent = testObject->AddComponent<PlayerComponent>();
		testObject->GetComponent<PlayerComponent>()->SetAudioManager(audioManager);
		auto* playerCollider = testObject->AddComponent < GE::SphereCollider >();
		playerCollider->SetCenter({ 0,0,0 });
		playerCollider->SetSize({ 20 });
		col1 = playerCollider;
	}
	PlayerComponent::dashMode = false;

	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("test2", "testTag"));
		testObject->GetTransform()->position = { 1300,0,0 };
		testObject->SetDrawAxisEnabled(true);
		auto* sampleCollider = testObject->AddComponent<GE::BoxCollider>();
		//auto* sampleComponent = testObject->AddComponent<GE::SampleComponent>();
		sampleCollider->SetCenter({ 0,0,0 });
		sampleCollider->SetSize({ 2 });
		sampleCollider->SetType(GE::ColliderType::OBB);
		col2 = sampleCollider;
	}

	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("miniMap", "miniMap"));
		auto* sampleComponent = testObject->AddComponent<MiniMapViewer>();
		sampleComponent->SetPlayer(gameObjectManager.FindGameObject("Player")->GetComponent<PlayerComponent>());
	}

	EnemyManager::GetInstance()->LoadPosition("Resources/enemies.txt");
	EnemyManager::GetInstance()->Start(&gameObjectManager);
	FieldObjectDebugTransform::GetInstance()->SetInputDevice(inputDevice);
	FieldObjectDebugTransform::GetInstance()->SetGraphicsDevice(graphicsDevice);
	FieldObjectManager::GetInstance()->LoadPosition("Resources/tree.txt");
	FieldObjectManager::GetInstance()->Start(&gameObjectManager);
	/*FieldObjectManager::GetInstance()->SetGroundMesh(groundModel);
	FieldObjectManager::GetInstance()->SetStartTreeMesh(startTreeModel);*/
	TimeLimit::GetInstance()->TimeSet = { 3,0 };
	TimeLimit::GetInstance()->Start(&gameObjectManager);
	//Collect::GetInstance()->Start(&gameObjectManager);

	collisionManager.AddTagCombination("player", "enemy");
	collisionManager.AddTagCombination("player", "frog");
	collisionManager.AddTagCombination("player", "ground");
	collisionManager.AddTagCombination("player", "StartTree");
	collisionManager.AddTagCombination("player", "nest");
	collisionManager.AddTagCombination("player", "tile");
	collisionManager.AddTagCombination("player", "fieldtree");
	collisionManager.AddTagCombination("frog", "tile");

	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("directionalLight", "directionalLight"));
		directionalLight = testObject->AddComponent<GE::DirectionalLight>();
		directionalLight->SetTarget(gameObjectManager.FindGameObjectWithTag("Player", "player")->GetTransform());
		testObject->GetTransform()->position = { 0,0,0 };
	}
}

void SampleScene::UnLoad()
{
	audioManager->Get("testBGM", 0)->Stop();

	Title::GetInstance()->ClearGameObject();
	EnemyManager::GetInstance()->UnLoad();
	FieldObjectManager::GetInstance()->UnLoad();
	FieldObjectDebugTransform::GetInstance()->UnLoad();
	UIObject::GetInstance()->UnLoad();
	PlayerColectObject::GetInstance()->ClearObject();
	ScreenUIManager::GetInstance()->UnLoad();
	// gameObjects‚ğíœ‚·‚é
	Scene::UnLoad();
}