#pragma once
#include"NormalEnemy.h"
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 

class EnemyManager
{
private:
	static EnemyManager* instance;
	GE::GameObjectManager* gameObjectManager = nullptr;
public:
	static EnemyManager* GetInstance();
	EnemyManager() = default;
	~EnemyManager() = default;
	void operator=(const EnemyManager& obj) = delete;
	EnemyManager(const EnemyManager& obj) = delete;
	void Start(int count);
	void SetGameObjectManager(GE::GameObjectManager* gameObjectManager_) { gameObjectManager = gameObjectManager_; }
};

