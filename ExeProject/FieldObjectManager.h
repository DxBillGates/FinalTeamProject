#pragma once
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics/MeshData.h>
#include <GatesEngine/Header/Graphics/Mesh.h>
#include <GatesEngine/Header/Graphics/VertexData.h>
#include "StartTree.h"


class FieldObjectManager
{
public:
	static GE::Math::Vector3 StartPosition;
private:
	struct  obj
	{
		GE::Math::Vector3 pos;
		GE::Math::Vector3 rot;
		GE::Math::Vector3 scale;
		GE::Color col;
	};
	GE::GameObjectManager* gameObjectManager = nullptr;
	std::vector<GE::GameObject*> fieldTree;
	std::vector<GE::GameObject*> birdChild;
	std::vector<GE::GameObject*> normalLeaf;
	GE::GameObject* fieldLeaf = nullptr;
	GE::GameObject* startTree = nullptr;
	GE::GameObject* nest = nullptr;
	GE::GameObject* tile = nullptr;
	GE::GameObject* skydome = nullptr;

	GE::IGraphicsDeviceDx12* graphicsDevice;
	GE::Mesh* mesh;
	std::map<std::string, GE::MeshData<GE::Vertex_UV_Normal>> groundModels;
	GE::MeshData<GE::Vertex_UV_Normal> startTreeModel;
	GE::MeshData<GE::Vertex_UV_Normal> tileModel;

private:
	void AddGroundModel(std::string fileName);
	void LoadModel(const std::string& filename);
public:
	static FieldObjectManager* GetInstance();
	void LoadModels();
	void Start(GE::GameObjectManager* gameObjectManager);
	void SetGraphicsDevice(GE::IGraphicsDeviceDx12* pGraphicsDevice) { graphicsDevice = pGraphicsDevice; }
	//�t�@�C���ɕۑ�����Ă�����W�擾
	void LoadPosition(const std::string& filename);
	//���݂̍��W���t�@�C���ɕۑ�
	void SaveCurrentPosition(const std::string& filename);

	//component���p�����Ă��Ȃ��X�V�̌Ăяo��
	void OtherUpdate();
	//component���p�����Ă��Ȃ��`��̌Ăяo��
	void OtherDraw();

	GE::GameObject* GetTile();

	void UnLoad();

private:
	FieldObjectManager() = default;

	~FieldObjectManager() = default;
	void operator=(const FieldObjectManager& obj) = delete;
	FieldObjectManager(const FieldObjectManager& obj) = delete;
};

