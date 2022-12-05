#include "FieldObjectManager.h"
#include "TestTreeComponent.h"
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
		auto* sampleComponent = object->AddComponent<TestTreeComponent>();
		auto* collider = object->AddComponent < GE::SphereCollider >();
		collider->SetCenter({ 0,0,0 });
		collider->SetSize({ 10 });
	}
}

void FieldObjectManager::SetGroundMesh(GE::MeshData<GE::Vertex_UV_Normal> mesh)
{
	//地形
	auto* object = gameObjectManager->AddGameObject(new GE::GameObject("ground", "ground"));
	auto* sampleComponent = object->AddComponent<Ground>();
	auto* collider = object->AddComponent < GE::MeshCollider >();
	collider->SetMesh(&mesh);
}
