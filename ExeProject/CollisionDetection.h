#pragma once
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/Util/Random.h>

struct CollisionInfo
{
	GE::GameObject* object;
	GE::ICollider* collider;
};

class CollisionDetection
{
private:
	static CollisionDetection* instance;

	//coll
public:
	static CollisionDetection* GetInstance();
	CollisionDetection() = default;
	~CollisionDetection() = default;
	void operator=(const CollisionDetection& obj) = delete;
	CollisionDetection(const CollisionDetection& obj) = delete;
	void Start();
};

