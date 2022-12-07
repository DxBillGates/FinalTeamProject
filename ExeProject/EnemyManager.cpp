#include "EnemyManager.h"
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>
#include<fstream>
#include<sstream>
#include<cassert>

EnemyManager* EnemyManager::GetInstance()
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Start(const int count, GE::GameObjectManager* gameObjectManager)
{
	this->count = count;
	this->gameObjectManager = gameObjectManager;
	for (int i = 0; i < 6; ++i)
	{
		auto* enemy = gameObjectManager->AddGameObject(new GE::GameObject("Enemy", "enemy"));
		auto* sampleComponent = enemy->AddComponent<NormalEnemy>();
		auto* normalEnemyCollider = enemy->AddComponent<GE::SphereCollider>();
		normalEnemyCollider->SetCenter({ 0,0,0 });
		normalEnemyCollider->SetSize({ 2 });
		nEnemies.push_back(enemy);
	}
	for (int i = 0; i < 0; ++i)
	{
		auto* bEnemy = gameObjectManager->AddGameObject(new GE::GameObject("BirdEnemy", "enemy"));
		auto* bComponent = bEnemy->AddComponent<BirdEnemy>();
		auto* birdEnemyCollider = bEnemy->AddComponent<GE::SphereCollider>();
		birdEnemyCollider->SetCenter({ 0,0,0 });
		birdEnemyCollider->SetSize({ 2 });
		birdEnemies.push_back(bEnemy);
	}
}

void EnemyManager::LoadPosition(const std::string& filename, std::vector<GE::GameObject*>enemies)
{
	std::vector<GE::Math::Vector3>pos;

	std::ifstream file;
	// .objファイルを開く
	file.open(filename);
	//ファイルが開けなかったらとまる
	if (file.fail()) {
		assert(0);
	}

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line)) {

		// 1行分の文字列
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "pos") {
			GE::Math::Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			pos.emplace_back(position);
		}
	}
	file.close();
	//ファイルの座標セット
	for (int i = 0; i < pos.size(); i++)
	{
		enemies[i]->GetTransform()->position = pos[i];
	}
}

void EnemyManager::SaveCurrentPosition(const std::string& filename, std::vector<GE::GameObject*>enemies)
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//内容初期化
	writing_file.clear();
	if (!writing_file.is_open()) { assert(0); }

	for (int i = 0; i < enemies.size(); i++)
	{
		GE::Math::Vector3 pos = enemies[i]->GetTransform()->position;
		writing_file << "pos " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	}

	writing_file.close();
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

