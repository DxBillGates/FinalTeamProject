#include "FieldObjectManager.h"
#include "TestTreeComponent.h"
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>

FieldObjectManager* FieldObjectManager::GetInstance()
{
	static FieldObjectManager instance;
	return &instance;
}

void FieldObjectManager::Start(GE::GameObjectManager* gameObjectManager)
{
	this->gameObjectManager = gameObjectManager;
	//�n�`
	{
		auto* object = gameObjectManager->AddGameObject(new GE::GameObject("Ground", "ground"));
		auto* sampleComponent = object->AddComponent<Ground>();
	}
	//�X�^�[�g�̎~�܂��
	{
		auto* object = gameObjectManager->AddGameObject(new GE::GameObject("Tree", "tree"));
		auto* sampleComponent = object->AddComponent<TestTreeComponent>();
	}
}
