#pragma once
#include"NormalEnemy.h"
#include"BirdEnemy.h"
#include"FlogEnemy.h"
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include <GatesEngine/Header/Util/Random.h>


class EnemyManager
{

private:
	struct  obj
	{
		GE::Math::Vector3 pos;
		GE::Math::Vector3 rot;
		GE::Math::Vector3 scale;
		GE::Color col;
	};
	GE::GameObjectManager* gameObjectManager = nullptr;
	std::vector<GE::GameObject*> nEnemies;
	std::vector<GE::GameObject*> birdEnemies;
	std::vector<GE::GameObject*> flogEnemies;

public:
	static EnemyManager* GetInstance();
	void Start(GE::GameObjectManager* gameObjectManager);
	//ファイルに保存されている座標取得
	void LoadPosition(const std::string& filename);
	//現在の敵たちの座標をファイルに保存
	void SaveCurrentPosition(const std::string& filename);

	void UnLoad();
	std::vector<GE::GameObject*> GetNormalEnemies() { return nEnemies; }
	std::vector<GE::GameObject*> GetBirdEnemies() { return birdEnemies; }
	std::vector<GE::GameObject*> GetFlogEnemies() { return flogEnemies; }
	std::vector<GE::GameObject*> GetAllEnemies();
private:
	EnemyManager() = default;
	~EnemyManager() = default;
	void operator=(const EnemyManager& obj) = delete;
	EnemyManager(const EnemyManager& obj) = delete;
};

