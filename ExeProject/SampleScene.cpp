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

	{
		MeshCreater::LoadObjModelData("Resources/Model/groundTest", groundModel);
		Mesh* mesh = new Mesh();
		mesh->Create(device, cmdList, groundModel);
		meshManager->Add(mesh, "ground");
	}
	{
		MeshCreater::LoadObjModelData("Resources/Model/tree2", startTreeModel);
		Mesh* mesh = new Mesh();
		mesh->Create(device, cmdList, startTreeModel);
		meshManager->Add(mesh, "startTree");
	}
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
	EnemyManager::GetInstance()->LoadPosition("Resources/enemies.txt");
	FieldObjectManager::GetInstance()->LoadPosition("Resources/tree.txt");
	UIObject::GetInstance()->Start();

}

void SampleScene::Update(float deltaTime)
{
	Title::GetInstance()->Select();
	gameObjectManager.Update(deltaTime);
	collisionManager.Update();
	Title::GetInstance()->Update();
	TimeLimit::GetInstance()->Update();
	Collect::GetInstance()->Update(StartTree::collectCount, StartTree::goalCollect);
	UIObject::GetInstance()->Update(deltaTime);
	//クリア以降条件
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::Q)
		|| StartTree::isCollect)
	{
		changeSceneInfo.flag = true;
		changeSceneInfo.name = "ClearScene";
		changeSceneInfo.initNextSceneFlag = true;
	}
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::E)
		|| TimeLimit::GetInstance()->GetTimeOver())
	{
		changeSceneInfo.flag = true;
		changeSceneInfo.name = "OverScene";
		changeSceneInfo.initNextSceneFlag = true;
	}

	Collect::GetInstance()->Update(StartTree::collectCount, StartTree::goalCollect);

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
		EnemyManager::GetInstance()->SaveCurrentPosition("Resources/enemies.txt");
		FieldObjectManager::GetInstance()->SaveCurrentPosition("Resources/tree.txt");
	}
}

void SampleScene::Draw()
{

	gameObjectManager.Draw();
	UIObject::GetInstance()->Draw(graphicsDevice);
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
		testObject->GetComponent<PlayerComponent>()->SetAudioManager(audioManager);
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
	EnemyManager::GetInstance()->Start(&gameObjectManager);
	FieldObjectManager::GetInstance()->Start(&gameObjectManager);
	FieldObjectManager::GetInstance()->SetGroundMesh(groundModel);
	FieldObjectManager::GetInstance()->SetStartTreeMesh(startTreeModel);
	TimeLimit::GetInstance()->Start(&gameObjectManager);
	Collect::GetInstance()->Start(&gameObjectManager);

	collisionManager.AddTagCombination("player", "enemy");
	collisionManager.AddTagCombination("player", "ground");
	collisionManager.AddTagCombination("player", "StartTree");
	collisionManager.AddTagCombination("player", "nest");
	collisionManager.AddTagCombination("player", "tile");
	//collisionManager.AddTagCombination("player", "birdEnemy");
}

void SampleScene::UnLoad()
{
	Title::GetInstance()->ClearGameObject();
	EnemyManager::GetInstance()->UnLoad();
	FieldObjectManager::GetInstance()->UnLoad();
	UIObject::GetInstance()->UnLoad();
	// gameObjectsを削除する
	Scene::UnLoad();
}