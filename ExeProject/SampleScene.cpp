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

SampleScene::SampleScene()
	: SampleScene("SampleScene")
{
}

SampleScene::SampleScene(const std::string& sceneName)
	: Scene(sceneName)
	, col1(nullptr)
	, col2(nullptr)
{
	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("Player","player"));
		auto* sampleComponent = testObject->AddComponent<PlayerComponent>();

		auto* playerCollider = testObject->AddComponent < GE::SphereCollider >();
		playerCollider->SetCenter({ 0,0,0 });
		playerCollider->SetSize({ 2 });
		col1 = playerCollider;
		//�^�C�g���Z�b�g
		Title::GetInstance()->Start(&gameObjectManager, testObject);
	}

	{
		/*auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("test2","testTag"));
		testObject->GetTransform()->position = { 1300,0,0 };
		testObject->SetDrawAxisEnabled(true);
		auto* sampleComponent = testObject->AddComponent<GE::SampleComponent>();*/
	}

	EnemyManager::GetInstance()->Start(10, &gameObjectManager);
	FieldObjectManager::GetInstance()->Start(&gameObjectManager);

	collisionManager.AddTagCombination("player", "enemy");
	//collisionManager.AddTagCombination("player", "birdEnemy");
}

SampleScene::~SampleScene()
{
}

void SampleScene::Initialize()
{
	gameObjectManager.Awake();
	gameObjectManager.Start();
}

void SampleScene::Update(float deltaTime)
{
	gameObjectManager.Update(deltaTime);
	collisionManager.Update();
	Title::GetInstance()->Update();

}

void SampleScene::Draw()
{
	gameObjectManager.Draw();
}

void SampleScene::LateDraw()
{
	gameObjectManager.LateDraw();
}
