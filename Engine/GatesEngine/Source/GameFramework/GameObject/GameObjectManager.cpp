#include "..\..\..\Header\GameFramework\GameObject\GameObjectManager.h"
#include "..\..\..\Header\GameFramework\Collision\CollisionManager.h"

GE::GameObjectManager::GameObjectManager()
{
}

GE::GameObjectManager::~GameObjectManager()
{
	DestroyGameObjects();
}

void GE::GameObjectManager::Awake()
{
	for (auto& objects : managedTagGameObjects)
	{
		for (auto& object : objects.second)
		{
			object->Awake();
		}
	}
}

void GE::GameObjectManager::Start()
{
	Awake();

	for (auto& gameObject : gameObjects)
	{
		gameObject->Start();
	}
}

void GE::GameObjectManager::Update(float deltaTime)
{
	// �폜�\��̃Q�[���I�u�W�F�N�g���폜
	Destroy();

	for (auto& gameObject : gameObjects)
	{
		gameObject->Update(deltaTime);

		// Hierarchy��GameObject��I��������Inspector�ɂ���GameObject��o�^
		if (hierarchyGui.OnGui(gameObject))
		{
			inspectorGui.SetCurrentSelectGameObject(gameObject);
		}
	}

	inspectorGui.OnGui();
}

void GE::GameObjectManager::DrawShadow()
{
	for (auto& objects : managedTagGameObjects)
	{
		for (auto& object : objects.second)
		{
			object->DrawShadow();
		}
	}
}

void GE::GameObjectManager::Draw()
{
	for (auto& gameObject : gameObjects)
	{
		gameObject->Draw();
	}
}

void GE::GameObjectManager::LateDraw()
{
	for (auto& gameObject : gameObjects)
	{
		gameObject->LateDraw();
	}
}

void GE::GameObjectManager::Destroy()
{
	if (destroyGameObjectCount == 0)return;

	// �o�^����Ă���^�O�𑖍�&�폜
	// remove_if��isDestroy��true�ɂȂ��Ă���I�u�W�F�N�g��z�񂩂�폜���Ă���
	for (auto& destroyTagObjects : managedTagGameObjects)
	{
		auto removeIfResult = std::remove_if(destroyTagObjects.second.begin(), destroyTagObjects.second.end(), [](GameObject* object)
			{
				bool isDestroy = object->IsDestroy();
				return isDestroy;
			});
		destroyTagObjects.second.erase(removeIfResult, destroyTagObjects.second.end());
	}

	// �^�O�͊Ǘ����Ă��Ȃ��z��̕����폜����
	// �������ŃQ�[���I�u�W�F�N�g���̂��폜����
	auto removeIfResult = std::remove_if(gameObjects.begin(), gameObjects.end(), [](GameObject* object)
		{
			bool isDestroy = object->IsDestroy();
			if (isDestroy)object->OnDestroy();
			return isDestroy;
		});
	gameObjects.erase(removeIfResult, gameObjects.end());


	destroyGameObjectCount = 0;
}

std::unordered_map<std::string, std::vector<GE::GameObject*>>* GE::GameObjectManager::GetManager()
{
	return &managedTagGameObjects;
}

GE::GameObject* GE::GameObjectManager::AddGameObject(GameObject* newGameObject)
{
	if (newGameObject == nullptr)return nullptr;

	managedTagGameObjects[newGameObject->GetTag()].emplace_back(newGameObject);
	gameObjects.emplace_back(newGameObject);

	newGameObject->SetGameObjectManager(this);

	return newGameObject;
}

GE::GameObject* GE::GameObjectManager::FindGameObject(const std::string& name)
{
	GameObject* returnObject = nullptr;

	for (auto& objects : managedTagGameObjects)
	{
		for (auto& object : objects.second)
		{
			if (object->GetName() != name)continue;

			returnObject = object;
			break;
		}
	}

	return returnObject;
}

GE::GameObject* GE::GameObjectManager::FindGameObjectWithTag(const std::string& name, const std::string& tag)
{
	GameObject* returnObject = nullptr;

	for (auto& objects : managedTagGameObjects)
	{
		for (auto& object : objects.second)
		{
			if (object->GetName() != name)continue;
			if (object->GetTag() != tag)continue;

			returnObject = object;
			break;
		}
	}

	return returnObject;
}

void GE::GameObjectManager::DestroyGameObject(const std::string& name)
{
	GameObject* destroyGameObject = FindGameObject(name);

	if (destroyGameObject == nullptr)return;

	DestroyGameObject(destroyGameObject);
}

void GE::GameObjectManager::DestroyGameObject(GameObject* gameObject)
{
	if (gameObject == nullptr)return;

	// ���̃t���[���J�n���ɍ폜�\��̃I�u�W�F�N�g�̐��𑝂₷
	++destroyGameObjectCount;

}

void GE::GameObjectManager::DestroyGameObjects()
{
	// �^�O�ƈꏏ�ɊǗ����Ă�z��̕�����폜����
	for (auto& tagGameObjects : managedTagGameObjects)
	{
		// ���ׂĂ̗v�f���폜�����邩��++it�͂���Ȃ�
		// erase�Ŏ���it���擾�ł��邩��
		for (auto it = tagGameObjects.second.begin(); it != tagGameObjects.second.end();)
		{
			it = tagGameObjects.second.erase(it);
		}
	}

	// �^�O�͊Ǘ����ĂȂ��z��̍폜����
	for (auto it = gameObjects.begin(); it != gameObjects.end();)
	{
		(*it)->OnDestroy();
		it = gameObjects.erase(it);
	}

}

bool GE::GameObjectManager::Raycast(const Math::Vector3& pos, const Math::Vector3& dir, const std::string& tag, float length, float* hitLenght)
{
	for (auto& objects : managedTagGameObjects)
	{
		if (objects.first != tag)continue;

		for (auto& object : objects.second)
		{
			ICollider* collider = object->GetCollider();
			if (collider == nullptr)continue;

			Math::Vector3 hitPos;

			bool isHit = CollisionManager::CheckSphereToRay(collider, object->GetTransform()->position, pos, dir, &hitPos);

			if (isHit == false)continue;
			if (hitPos.Length() > length)continue;

			// �R���C�_�[�̐F��ύX������悤
			collider->Hit();

			return true;
		}
	}
	return false;
}

