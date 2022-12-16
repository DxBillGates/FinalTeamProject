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

void EnemyManager::Start(GE::GameObjectManager* gameObjectManager)
{
	this->gameObjectManager = gameObjectManager;
	for (int i = 0; i < 10; ++i)
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

void EnemyManager::LoadPosition(const std::string& filename)
{
	std::vector<obj>ne;
	std::vector<obj>be;

	std::ifstream file;
	//ファイルを開く
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

		if (key == "normalEnemy") {
			obj result;
			line_stream >> result.pos.x;
			line_stream >> result.pos.y;
			line_stream >> result.pos.z;

			line_stream >> result.rot.x;
			line_stream >> result.rot.y;
			line_stream >> result.rot.z;

			line_stream >> result.scale.x;
			line_stream >> result.scale.y;
			line_stream >> result.scale.z;

			line_stream >> result.col.r;
			line_stream >> result.col.g;
			line_stream >> result.col.b;
			result.col.a = 1.0f;
			ne.emplace_back(result);
		}
		else if (key == "birdEnemy") {
			obj result;
			line_stream >> result.pos.x;
			line_stream >> result.pos.y;
			line_stream >> result.pos.z;

			line_stream >> result.rot.x;
			line_stream >> result.rot.y;
			line_stream >> result.rot.z;

			line_stream >> result.scale.x;
			line_stream >> result.scale.y;
			line_stream >> result.scale.z;

			line_stream >> result.col.r;
			line_stream >> result.col.g;
			line_stream >> result.col.b;
			result.col.a = 1.0f;
			be.emplace_back(result);
		}
	}
	file.close();
	//ファイルの座標セット
	int index = ne.size() < nEnemies.size() ? ne.size() : nEnemies.size();
	for (int i = 0; i < index; i++)
	{
		nEnemies[i]->GetTransform()->position = ne[i].pos;
		//nEnemies[i]->GetComponent<FieldTree>()->rotation_euler = ft[i].rot;
		nEnemies[i]->GetTransform()->scale = ne[i].scale;
	}
	index = be.size() < birdEnemies.size() ? be.size() : birdEnemies.size();
	for (int i = 0; i < index; i++)
	{
		birdEnemies[i]->GetTransform()->position = be[i].pos;
		//nEnemies[i]->GetComponent<FieldTree>()->rotation_euler = ft[i].rot;
		birdEnemies[i]->GetTransform()->scale = be[i].scale;
	}
}

void EnemyManager::SaveCurrentPosition(const std::string& filename)
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//内容初期化
	writing_file.clear();
	if (!writing_file.is_open()) { assert(0); }

	for (int i = 0; i < nEnemies.size(); i++)
	{
		GE::Math::Vector3 pos = nEnemies[i]->GetTransform()->position;
		GE::Math::Vector3 scale = nEnemies[i]->GetTransform()->scale;
		GE::Math::Vector3 rota = {};//nEnemies[i]->GetComponent<Enemy>()->rotation_euler;
		GE::Color col = nEnemies[i]->GetColor();

		writing_file << "normalEnemy " << pos.x << " " << pos.y << " " << pos.z <<
			" " << rota.x << " " << rota.y << " " << rota.z <<
			" " << scale.x << " " << scale.y << " " << scale.z <<
			" " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;
	}

	for (int i = 0; i < birdEnemies.size(); i++)
	{
		GE::Math::Vector3 pos = birdEnemies[i]->GetTransform()->position;
		GE::Math::Vector3 scale = birdEnemies[i]->GetTransform()->scale;
		GE::Math::Vector3 rota = {};// nEnemies[i]->GetComponent<Enemy>()->rotation_euler;
		GE::Color col = birdEnemies[i]->GetColor();

		writing_file << "birdEnemy " << pos.x << " " << pos.y << " " << pos.z <<
			" " << rota.x << " " << rota.y << " " << rota.z <<
			" " << scale.x << " " << scale.y << " " << scale.z <<
			" " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;
	}

	writing_file.close();
}

void EnemyManager::UnLoad()
{
	nEnemies.clear();
	birdEnemies.clear();
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

