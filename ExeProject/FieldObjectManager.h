#pragma once
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics/MeshData.h>
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
	GE::GameObject* startTree = nullptr;
	GE::GameObject* nest = nullptr;
	GE::GameObject* skydome = nullptr;

public:
	static FieldObjectManager* GetInstance();
	void Start(GE::GameObjectManager* gameObjectManager);
	void SetGroundMesh(GE::MeshData<GE::Vertex_UV_Normal> mesh);
	void SetStartTreeMesh(GE::MeshData<GE::Vertex_UV_Normal> mesh);
	//ファイルに保存されている座標取得
	void LoadPosition(const std::string& filename);
	//現在の座標をファイルに保存
	void SaveCurrentPosition(const std::string& filename);

	void UnLoad();
private:
	FieldObjectManager() = default;

	~FieldObjectManager() = default;
	void operator=(const FieldObjectManager& obj) = delete;
	FieldObjectManager(const FieldObjectManager& obj) = delete;
};

