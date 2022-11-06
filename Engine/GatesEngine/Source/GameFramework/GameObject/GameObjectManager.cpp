#include "..\..\..\Header\GameFramework\GameObject\GameObjectManager.h"

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
	for (auto& objects : gameObjects)
	{
		for (auto& object : objects.second)
		{
			object->Update(deltaTime);

			// Hierarchy‚ÌGameObject‚ð‘I‘ð‚µ‚½‚çInspector‚É‚»‚ÌGameObject‚ð“o˜^
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

	gameObjects.insert(std::make_pair(tag,std::vector<GameObject*>()));
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
