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
	// 削除予定のゲームオブジェクトを削除
	Destroy();

	for (auto& gameObject : gameObjects)
	{
		gameObject->Update(deltaTime);

		// HierarchyのGameObjectを選択したらInspectorにそのGameObjectを登録
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

	// 登録されているタグを走査&削除
	// remove_ifでisDestroyがtrueになっているオブジェクトを配列から削除していく
	for (auto& destroyTagObjects : managedTagGameObjects)
	{
		auto removeIfResult = std::remove_if(destroyTagObjects.second.begin(), destroyTagObjects.second.end(), [](GameObject* object)
			{
				bool isDestroy = object->IsDestroy();
				return isDestroy;
			});
		destroyTagObjects.second.erase(removeIfResult, destroyTagObjects.second.end());
	}

	// タグは管理していない配列の方も削除する
	// こっちでゲームオブジェクト自体も削除する
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

	// 次のフレーム開始時に削除予定のオブジェクトの数を増やす
	++destroyGameObjectCount;

}

void GE::GameObjectManager::DestroyGameObjects()
{
	// タグと一緒に管理してる配列の方から削除処理
	for (auto& tagGameObjects : managedTagGameObjects)
	{
		// すべての要素を削除させるから++itはいらない
		// eraseで次のitを取得できるから
		for (auto it = tagGameObjects.second.begin(); it != tagGameObjects.second.end();)
		{
			it = tagGameObjects.second.erase(it);
		}
	}

	// タグは管理してない配列の削除処理
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

			// コライダーの色を変更させるよう
			collider->Hit();

			return true;
		}
	}
	return false;
}

