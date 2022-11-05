#pragma once
#include"NormalEnemy.h"
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/Util/Random.h>


class EnemyManager
{
private:
	static EnemyManager* instance;
	GE::GameObjectManager* gameObjectManager = nullptr;

	unsigned int count;
public:
	static EnemyManager* GetInstance();
	EnemyManager() = default;
	~EnemyManager() = default;
	void operator=(const EnemyManager& obj) = delete;
	EnemyManager(const EnemyManager& obj) = delete;
	void Start(const int count);
	void SetGameObjectManager(GE::GameObjectManager* gameObjectManager_) { gameObjectManager = gameObjectManager_; }
};

