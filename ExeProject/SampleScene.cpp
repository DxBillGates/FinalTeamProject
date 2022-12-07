#include "SampleScene.h"
#include "PlayerComponent.h"
#include"EnemyManager.h"
#include"FieldObjectManager.h"
#include"TestTreeComponent.h"
#include <GatesEngine/Header\GameFramework\Component\SampleComponent.h>
#include <GatesEngine/Header\GameFramework\Component\SphereCollider.h>
#include <GatesEngine/Header\GameFramework\Component\BoxCollider.h>
#include <GatesEngine/Header\GameFramework\Collision\CollisionManager.h>
#include <GatesEngine/Header/Application/Application.h>
#include"Title.h"
#include "MiniMapViewer.h"

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
	using namespace GE;
	auto meshManager = graphicsDevice->GetMeshManager();
	auto device = graphicsDevice->GetDevice();
	auto cmdList = graphicsDevice->GetCmdList();


	MeshCreater::LoadObjModelData("Resources/Model/groundTest", groundModel);
	Mesh* mesh = new Mesh();
	mesh->Create(device, cmdList, groundModel);
	meshManager->Add(mesh, "ground");
}

SampleScene::~SampleScene()
{
}

void SampleScene::Initialize()
{
	Title::GetInstance()->Awake(&gameObjectManager, graphicsDevice);

	gameObjectManager.Awake();
	gameObjectManager.Start();
}

void SampleScene::Update(float deltaTime)
{
	gameObjectManager.Update(deltaTime);
	collisionManager.Update();
	Title::GetInstance()->Update();

	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::Q))
	{
		changeSceneInfo.name = "ClearScene";
		changeSceneInfo.flag = true;
		changeSceneInfo.initNextSceneFlag = true;
	}
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::Y))
	{
		changeSceneInfo.name = "SampleScene";
		changeSceneInfo.flag = true;
		changeSceneInfo.initNextSceneFlag = true;
	}
}

void SampleScene::Draw()
{
	gameObjectManager.Draw();
}

void SampleScene::LateDraw()
{
	gameObjectManager.LateDraw();
}

void SampleScene::Load()
{
	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("Player", "player"));
		auto* sampleComponent = testObject->AddComponent<PlayerComponent>();

		auto* playerCollider = testObject->AddComponent < GE::SphereCollider >();
		playerCollider->SetCenter({ 0,0,0 });
		playerCollider->SetSize({ 20 });
		col1 = playerCollider;

	}

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

	EnemyManager::GetInstance()->Start(10, &gameObjectManager);
	FieldObjectManager::GetInstance()->Start(&gameObjectManager);
	FieldObjectManager::GetInstance()->SetGroundMesh(groundModel);

	collisionManager.AddTagCombination("player", "enemy");
	collisionManager.AddTagCombination("player", "ground");
	collisionManager.AddTagCombination("player", "tree");
	//collisionManager.AddTagCombination("player", "birdEnemy");
}

void SampleScene::UnLoad()
{
	Title::GetInstance()->ClearGameObject();

	// gameObjects‚ğíœ‚·‚é
	Scene::UnLoad();
}
