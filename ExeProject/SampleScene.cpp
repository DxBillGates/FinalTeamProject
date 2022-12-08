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
#include"TimeLimit.h"
#include"MiniMapViewer.h"
#include"Collect.h"

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
	//ノーマルエネミー座標ファイル読み込み、座標反映
	EnemyManager::GetInstance()->LoadPosition("Resources/normalEnemies.txt", EnemyManager::GetInstance()->GetNormalEnemies());
}

void SampleScene::Update(float deltaTime)
{
	gameObjectManager.Update(deltaTime);
	collisionManager.Update();
	Title::GetInstance()->Update();
	TimeLimit::GetInstance()->Update();
	Collect::GetInstance()->Update(0);

	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::Y))
	{
		changeSceneInfo.flag = true;
		changeSceneInfo.name = "SampleScene";
		changeSceneInfo.initNextSceneFlag = true;
	}

	// LCtr + S で敵の位置保存
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::LCONTROL) &&
		inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::S))
	{
		EnemyManager::GetInstance()->SaveCurrentPosition("Resources/normalEnemies.txt", EnemyManager::GetInstance()->GetNormalEnemies());
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
	TimeLimit::GetInstance()->Start(&gameObjectManager);
	Collect::GetInstance()->Start(&gameObjectManager);

	collisionManager.AddTagCombination("player", "enemy");
	collisionManager.AddTagCombination("player", "ground");
	collisionManager.AddTagCombination("player", "tree");
	//collisionManager.AddTagCombination("player", "birdEnemy");
}

void SampleScene::UnLoad()
{
	// gameObjectsを削除する
	Scene::UnLoad();
}