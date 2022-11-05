#include "EnemyManager.h"
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>

EnemyManager* EnemyManager::instance;
EnemyManager* EnemyManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new EnemyManager();
	}

	return instance;
}

void EnemyManager::Start(const int count)
{
	this->count = count;
	for (int i = 0; i < this->count; ++i)
	{
		auto* enemy = gameObjectManager->AddGameObject(new GE::GameObject());
		enemy->SetName("Enemy");
		auto* sampleComponent = enemy->AddComponent<NormalEnemy>();
		auto* normalEnemyCollider = enemy->AddComponent<GE::SphereCollider>();
		normalEnemyCollider->SetCenter({ 0,0,0 });
		normalEnemyCollider->SetSize({ 1 });
	}
}

