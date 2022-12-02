#pragma once
#include"NormalEnemy.h"
#include"Ground.h"
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include <GatesEngine/Header/Util/Random.h>


class FieldObjectManager
{

private:
	GE::GameObjectManager* gameObjectManager = nullptr;
public:
	static FieldObjectManager* GetInstance();
	FieldObjectManager() = default;
	~FieldObjectManager() = default;
	void operator=(const FieldObjectManager& obj) = delete;
	FieldObjectManager(const FieldObjectManager& obj) = delete;
	void Start(GE::GameObjectManager* gameObjectManager);
};
