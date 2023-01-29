#include "EnemyManager.h"
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>
#include<fstream>
#include<sstream>
#include<cassert>
#include "FieldObjectDeBugTransform.h"

EnemyManager* EnemyManager::GetInstance()
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Start(GE::GameObjectManager* gameObjectManager)
{
	this->gameObjectManager = gameObjectManager;
	for (int i = 0; i < ne.size(); ++i)
	{
		auto* enemy = gameObjectManager->AddGameObject(new GE::GameObject("Enemy", "enemy"));
		auto* sampleComponent = enemy->AddComponent<NormalEnemy>();
		enemy->GetTransform()->position = ne[i].pos;
		enemy->GetTransform()->scale = ne[i].scale;
		auto* normalEnemyCollider = enemy->AddComponent<GE::SphereCollider>();
		normalEnemyCollider->SetCenter({ 0,0,0 });
		normalEnemyCollider->SetSize({ 40 });
		nEnemies.push_back(enemy);
		FieldObjectDebugTransform::GetInstance()->AddTarget(enemy, { 100,100,100 });

	}
	for (int i = 0; i < be.size(); ++i)
	{
		auto* bEnemy = gameObjectManager->AddGameObject(new GE::GameObject("BirdEnemy", "enemy"));
		auto* bComponent = bEnemy->AddComponent<BirdEnemy>();
		auto* birdEnemyCollider = bEnemy->AddComponent<GE::SphereCollider>();
		birdEnemyCollider->SetCenter({ 0,0,0 });
		birdEnemyCollider->SetSize({ 2 });
		birdEnemies.push_back(bEnemy);
	}
	for (int i = 0; i < fe.size(); ++i)
	{
		auto* fEnemy = gameObjectManager->AddGameObject(new GE::GameObject("FrogEnemy", "frog"));
		auto* fComponent = fEnemy->AddComponent<FrogEnemy>();
		fEnemy->GetTransform()->position = fe[i].pos;
		fEnemy->GetTransform()->scale = fe[i].scale;
		auto* flogEnemyCollider = fEnemy->AddComponent<GE::SphereCollider>();
		flogEnemyCollider->SetCenter({ 0,40,0 });
		flogEnemyCollider->SetSize({ 80 });
		frogEnemies.push_back(fEnemy);
		FieldObjectDebugTransform::GetInstance()->AddTarget(fEnemy, { 100,100,100 });
	}
}

void EnemyManager::LoadPosition(const std::string& filename)
{
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
		else if (key == "frogEnemy") {
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
			fe.emplace_back(result);
		}
	}
	file.close();
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
		GE::Math::Vector3 rota = {};
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
		GE::Math::Vector3 rota = {};
		GE::Color col = birdEnemies[i]->GetColor();

		writing_file << "birdEnemy " << pos.x << " " << pos.y << " " << pos.z <<
			" " << rota.x << " " << rota.y << " " << rota.z <<
			" " << scale.x << " " << scale.y << " " << scale.z <<
			" " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;
	}
	for (int i = 0; i < frogEnemies.size(); i++)
	{
		GE::Math::Vector3 pos = frogEnemies[i]->GetTransform()->position;
		GE::Math::Vector3 scale = frogEnemies[i]->GetTransform()->scale;
		GE::Math::Vector3 rota = {};
		GE::Color col = frogEnemies[i]->GetColor();

		//カエルは重力が効いてるからY座標めり込まないように500にしてるよ
		writing_file << "frogEnemy " << pos.x << " " << 500 << " " << pos.z <<
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
	frogEnemies.clear();
}

std::vector<GE::GameObject*> EnemyManager::GetAllEnemies()
{
	std::vector<GE::GameObject*> result;

	auto manager = gameObjectManager->GetManager();
	for (auto& ne : (*manager)["enemy"])
	{
		result.push_back(ne);
	}
	/*for (auto& be : (*manager)["birdEnemy"])
	{
		result.push_back(be);
	}*/
	for (auto& fe : (*manager)["frog"])
	{
		result.push_back(fe);
	}
	return result;
}

