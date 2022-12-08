#include "FieldObjectManager.h"
#include "StartTree.h"
#include "FieldObject.h"
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>
#include <GatesEngine/Header/GameFramework/Component/MeshCollider.h>
#include<fstream>
#include<sstream>
#include<cassert>

FieldObjectManager* FieldObjectManager::GetInstance()
{
	static FieldObjectManager instance;
	return &instance;
}

void FieldObjectManager::Start(GE::GameObjectManager* gameObjectManager)
{
	this->gameObjectManager = gameObjectManager;

	//�X�^�[�g�̎~�܂��
	{
		auto* object = gameObjectManager->AddGameObject(new GE::GameObject("Tree", "tree"));
		auto* sampleComponent = object->AddComponent<StartTree>();
		auto* collider = object->AddComponent < GE::SphereCollider >();
		collider->SetCenter({ 0,0,0 });
		collider->SetSize({ 10 });
	}
	//�ǉ�
	{
		/*auto* object = gameObjectManager->AddGameObject(new GE::GameObject("skydome", "skydome"));
		auto* sampleComponent = object->AddComponent<FieldObject>();
		object->GetComponent<FieldObject>()->modelName = "Skydome";
		object->GetComponent<FieldObject>()->shaderName = "DefaultMeshShader";
		object->SetColor(GE::Color(1.f, 1.f, 1.f, 0.4f));
		object->GetTransform()->position = StartTree::position;
		object->GetTransform()->scale = { 400 };*/
	}
}

void FieldObjectManager::SetGroundMesh(GE::MeshData<GE::Vertex_UV_Normal> mesh)
{
	//�n�`
	auto* object = gameObjectManager->AddGameObject(new GE::GameObject("ground", "ground"));
	auto* sampleComponent = object->AddComponent<FieldObject>();
	auto* collider = object->AddComponent < GE::MeshCollider >();
	collider->SetMesh(&mesh);
	object->GetComponent<FieldObject>()->modelName = "GroundTest";
	object->SetColor(GE::Color(0.5f, 0.9f, 0.5f, 1.0f));
	object->GetTransform()->position = { 1000,0,-15000 };
	object->GetTransform()->scale = { 2 };
	object->GetTransform()->rotation = GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), -5.0f);
}

void FieldObjectManager::LoadPosition(const std::string& filename, std::vector<GE::GameObject*>objs)
{
	std::vector<GE::Math::Vector3>pos;

	std::ifstream file;
	// .obj�t�@�C�����J��
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

		if (key == "pos") {
			GE::Math::Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			pos.emplace_back(position);
		}
	}
	file.close();
	//�t�@�C���̍��W�Z�b�g
	int index = pos.size() < objs.size() ? pos.size() : objs.size();
	for (int i = 0; i < index; i++)
	{
		objs[i]->GetTransform()->position = pos[i];
	}
}

void FieldObjectManager::SaveCurrentPosition(const std::string& filename, std::vector<GE::GameObject*>objs)
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//���e������
	writing_file.clear();
	if (!writing_file.is_open()) { assert(0); }

	for (int i = 0; i < objs.size(); i++)
	{
		GE::Math::Vector3 pos = objs[i]->GetTransform()->position;
		writing_file << "pos " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	}

	writing_file.close();
}