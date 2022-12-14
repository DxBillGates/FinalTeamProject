#include "..\..\..\Header\GameFramework\GameObject\GameObjectManager.h"
#include "..\..\..\Header\GameFramework\Collision\CollisionManager.h"

GE::GameObjectManager::GameObjectManager()
{
}

GE::GameObjectManager::~GameObjectManager()
{
	for (auto& objects : gameObjects)
	{
		for (auto& object : objects.second)
		{
			delete object;
		}
	}
	gameObjects.clear();
}

void GE::GameObjectManager::Awake()
{
	for (auto& objects : gameObjects)
	{
		for (auto& object : objects.second)
		{
			object->Awake();
		}
	}
}

void GE::GameObjectManager::Start()
{
	for (auto& objects : gameObjects)
	{
		for (auto& object : objects.second)
		{
			object->Start();
		}
	}
}

void GE::GameObjectManager::Update(float deltaTime)
{
	// 前フレームに削除予定配列に登録されたゲームオブジェクトを削除しつつその配列をクリア
	Destroy();

	for (auto& objects : gameObjects)
	{
		for (auto& object : objects.second)
		{
			object->Update(deltaTime);

			// HierarchyのGameObjectを選択したらInspectorにそのGameObjectを登録
			if (hierarchyGui.OnGui(object))
			{
				inspectorGui.SetCurrentSelectGameObject(object);
			}
		}
	}
	inspectorGui.OnGui();
}

void GE::GameObjectManager::Draw()
{
	for (auto& objects : gameObjects)
	{
		for (auto& object : objects.second)
		{
			object->Draw();
		}
	}
}

void GE::GameObjectManager::LateDraw()
{
	for (auto& objects : gameObjects)
	{
		for (auto& object : objects.second)
		{
			object->LateDraw();
		}
	}
}

void GE::GameObjectManager::Destroy()
{
	if (destroyGameObjects.size() == 0)return;

	for (auto& destroyTagObjects : destroyGameObjects)
	{
		// そのタグで登録されているゲームオブジェクトを削除していく
		for (auto& destroyObject : destroyTagObjects.second)
		{
			std::string tag = destroyTagObjects.first;
			for (auto& gameObject : gameObjects[tag])
			{
				if (destroyObject == gameObject)
				{
					if (gameObject == inspectorGui.CurrentSelectGameObject())
					{
						inspectorGui.SetCurrentSelectGameObject(nullptr);
					}

					gameObject->OnDestroy();
					delete gameObject;
					gameObject = nullptr;
					std::swap(gameObject, gameObjects[tag].back());
					gameObjects[tag].pop_back();
					break;
				}
			}
		}
		destroyTagObjects.second.clear();
	}
	destroyGameObjects.clear();
}

std::map<std::string, std::vector<GE::GameObject*>>* GE::GameObjectManager::GetManager()
{
	return &gameObjects;
}

GE::GameObject* GE::GameObjectManager::AddGameObject(GameObject* newGameObject)
{
	const std::string& tag = newGameObject->GetTag();

	for (auto& objects : gameObjects)
	{
		if (objects.first == tag)
		{
			objects.second.push_back(newGameObject);
			newGameObject->SetGameObjectManager(this);
			return newGameObject;
		}
	}

	gameObjects.insert(std::make_pair(tag, std::vector<GameObject*>()));
	gameObjects[tag].push_back(newGameObject);
	newGameObject->SetGameObjectManager(this);

	return newGameObject;
}

GE::GameObject* GE::GameObjectManager::FindGameObject(const std::string& name)
{
	GameObject* returnObject = nullptr;

	for (auto& objects : gameObjects)
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

	for (auto& objects : gameObjects)
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
	std::string tag = gameObject->GetTag();

	// これから削除する予定のオブジェクトリストにそのタグが登録されているかを確認
	// されていなかったら登録
	if (destroyGameObjects.find(tag) == destroyGameObjects.end())
	{
		destroyGameObjects.insert(std::make_pair(tag, std::vector<GameObject*>()));
	}

	destroyGameObjects[tag].push_back(gameObject);

}

void GE::GameObjectManager::DestroyGameObjects()
{
	for (auto& tagGameObjects : gameObjects)
	{
		for (auto& tagGameObject : tagGameObjects.second)
		{
			DestroyGameObject(tagGameObject);
		}
	}

	Destroy();
}

bool GE::GameObjectManager::Raycast(const Math::Vector3& pos, const Math::Vector3& dir, const std::string& tag, float length, float* hitLenght)
{
	for (auto& objects : gameObjects)
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

			// コライダーの色を変更させるよう
			collider->Hit();

			return true;
		}
	}
	return false;
}

