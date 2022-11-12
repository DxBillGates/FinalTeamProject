#include "EnemyManager.h"
#include "CollisionDetection.h"
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>

EnemyManager* EnemyManager::GetInstance()
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Start(const int count, GE::GameObjectManager* gameObjectManager)
{
	this->count = count;
	this->gameObjectManager = gameObjectManager;
	for (int i = 0; i < 5; ++i)
	{
		auto* enemy = gameObjectManager->AddGameObject(new GE::GameObject("Enemy", "enemy"));
		auto* sampleComponent = enemy->AddComponent<NormalEnemy>();
		auto* normalEnemyCollider = enemy->AddComponent<GE::SphereCollider>();
		normalEnemyCollider->SetCenter({ 0,0,0 });
		normalEnemyCollider->SetSize({ 2 });
		nEnemies.push_back(enemy);
		CollisionDetection::GetInstance()->SetNormalEnemies(enemy, normalEnemyCollider);
	}
	for (int i = 0; i < 10; ++i)
	{
		auto* bEnemy = gameObjectManager->AddGameObject(new GE::GameObject("BirdEnemy", "birdEnemy"));
		auto* bComponent = bEnemy->AddComponent<BirdEnemy>();
		auto* birdEnemyCollider = bEnemy->AddComponent<GE::SphereCollider>();
		birdEnemyCollider->SetCenter({ 0,0,0 });
		birdEnemyCollider->SetSize({ 2 });
		birdEnemies.push_back(bEnemy);
		CollisionDetection::GetInstance()->SetBirdEnemies(bEnemy, birdEnemyCollider);
	}

}

std::vector<GE::GameObject*> EnemyManager::GetAllEnemies()
{
	std::vector<GE::GameObject*> result;

	auto manager = gameObjectManager->GetManager();
	for (auto& ne : (*manager)["enemy"])
	{
		result.push_back(ne);
	}
	for (auto& ne : (*manager)["birdEnemy"])
	{
		result.push_back(ne);
	}
	return result;
}

