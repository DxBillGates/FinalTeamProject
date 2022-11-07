#pragma once
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 

struct CollisionObjectData
{
	GE::GameObject* object;
	GE::ICollider* collider;
};

class CollisionDetection
{
private:
	CollisionObjectData player;
	CollisionObjectData lockon;
	std::vector<CollisionObjectData> nEnemies;
public:
	static CollisionDetection* GetInstance();
	CollisionDetection() = default;
	~CollisionDetection() = default;
	void operator=(const CollisionDetection& obj) = delete;
	CollisionDetection(const CollisionDetection& obj) = delete;
	void Update(float deltaTime);

	void SetPlayer(GE::GameObject* p, GE::ICollider* coll);
	void SetLockOn(GE::GameObject* p, GE::ICollider* coll);
	void SetNormalEnemies(GE::GameObject* ne, GE::ICollider* coll);
};

