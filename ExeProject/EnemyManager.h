#pragma once
#include"NormalEnemy.h"
#include"BirdEnemy.h"
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include <GatesEngine/Header/Util/Random.h>


class EnemyManager
{

private:
	GE::GameObjectManager* gameObjectManager = nullptr;
	std::vector<GE::GameObject*> nEnemies;
	std::vector<GE::GameObject*> birdEnemies;
	unsigned int count = 0;
public:
	static EnemyManager* GetInstance();
	EnemyManager() = default;
	~EnemyManager() = default;
	void operator=(const EnemyManager& obj) = delete;
	EnemyManager(const EnemyManager& obj) = delete;
	void Start(const int count, GE::GameObjectManager* gameObjectManager);

	std::vector<GE::GameObject*> GetNormalEnemies() { return nEnemies; }
	std::vector<GE::GameObject*> GetBirdEnemies() { return birdEnemies; }
	std::vector<GE::GameObject*> GetAllEnemies();
};

