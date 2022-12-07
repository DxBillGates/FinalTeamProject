#include "FieldObjectManager.h"
#include "StartTree.h"
#include "FieldObject.h"
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>
#include <GatesEngine/Header/GameFramework/Component/MeshCollider.h>

FieldObjectManager* FieldObjectManager::GetInstance()
{
	static FieldObjectManager instance;
	return &instance;
}

void FieldObjectManager::Start(GE::GameObjectManager* gameObjectManager)
{
	this->gameObjectManager = gameObjectManager;

	//スタートの止まり木
	{
		auto* object = gameObjectManager->AddGameObject(new GE::GameObject("Tree", "tree"));
		auto* sampleComponent = object->AddComponent<StartTree>();
		auto* collider = object->AddComponent < GE::SphereCollider >();
		collider->SetCenter({ 0,0,0 });
		collider->SetSize({ 10 });
	}
	//壁仮
	{
		auto* object = gameObjectManager->AddGameObject(new GE::GameObject("skydome", "skydome"));
		auto* sampleComponent = object->AddComponent<FieldObject>();
		object->GetComponent<FieldObject>()->modelName = "Skydome";
		object->GetComponent<FieldObject>()->shaderName = "DefaultMeshShader";
		object->SetColor(GE::Color(1.f, 1.f, 1.f, 0.4f));
		object->GetTransform()->position = StartTree::position;
		object->GetTransform()->scale = { 400 };
	}
}

void FieldObjectManager::SetGroundMesh(GE::MeshData<GE::Vertex_UV_Normal> mesh)
{
	//地形
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
