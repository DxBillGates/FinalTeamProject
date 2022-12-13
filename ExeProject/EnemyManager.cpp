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
	for (int i = 0; i < 7; ++i)
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
	std::vector<GE::Math::Vector3>ne;
	std::vector<GE::Math::Vector3>be;

	std::ifstream file;
	//�t�@�C�����J��
	file.open(filename);
	//�t�@�C�����J���Ȃ�������Ƃ܂�
	if (file.fail()) {
		assert(0);
	}

	// 1�s���ǂݍ���
	std::string line;
	while (getline(file, line)) {

		// 1�s���̕�����
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "normalEnemy") {
			GE::Math::Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			ne.emplace_back(position);
		}
		else if (key == "birdEnemy") {
			GE::Math::Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			be.emplace_back(position);
		}
	}
	file.close();
	//�t�@�C���̍��W�Z�b�g
	int index = ne.size() < nEnemies.size() ? ne.size() : nEnemies.size();
	for (int i = 0; i < index; i++)
	{
		nEnemies[i]->GetTransform()->position = ne[i];
	}
	index = be.size() < birdEnemies.size() ? be.size() : birdEnemies.size();
	for (int i = 0; i < index; i++)
	{
		birdEnemies[i]->GetTransform()->position = be[i];
	}
}

void EnemyManager::SaveCurrentPosition(const std::string& filename)
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//���e������
	writing_file.clear();
	if (!writing_file.is_open()) { assert(0); }

	for (int i = 0; i < nEnemies.size(); i++)
	{
		GE::Math::Vector3 pos = nEnemies[i]->GetTransform()->position;
		writing_file << "normalEnemy " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	}
	
	for (int i = 0; i < birdEnemies.size(); i++)
	{
		GE::Math::Vector3 pos = birdEnemies[i]->GetTransform()->position;
		writing_file << "birdEnemy " << pos.x << " " << pos.y << " " << pos.z << std::endl;
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

