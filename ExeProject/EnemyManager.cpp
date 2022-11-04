#include "EnemyManager.h"

EnemyManager* EnemyManager::instance;
EnemyManager* EnemyManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new EnemyManager();
    }

    return instance;
}

void EnemyManager::Start(int count)
{
    for (int i = 0; i < count; ++i)
    {
        auto* testObject = gameObjectManager->AddGameObject(new GE::GameObject());
        testObject->SetName("Enemy");
        auto* sampleComponent = testObject->AddComponent<NormalEnemy>();
    }
}

