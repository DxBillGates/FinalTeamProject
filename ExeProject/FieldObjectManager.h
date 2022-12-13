#pragma once
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics/MeshData.h>
#include <GatesEngine/Header/Graphics/VertexData.h>
#include "StartTree.h"


class FieldObjectManager
{

private:
	GE::GameObjectManager* gameObjectManager = nullptr;
	//std::vector<GE::GameObject*> nEnemies;
	std::vector<GE::GameObject*> fieldTree;
public:
	static FieldObjectManager* GetInstance();
	FieldObjectManager() = default;
	~FieldObjectManager() = default;
	void operator=(const FieldObjectManager& obj) = delete;
	FieldObjectManager(const FieldObjectManager& obj) = delete;
	void Start(GE::GameObjectManager* gameObjectManager);
	void SetGroundMesh(GE::MeshData<GE::Vertex_UV_Normal> mesh);
	//�t�@�C���ɕۑ�����Ă�����W�擾
	void LoadPosition(const std::string& filename);
	//���݂̓G�����̍��W���t�@�C���ɕۑ�
	void SaveCurrentPosition(const std::string& filename);

};

