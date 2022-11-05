#include "CollisionDetection.h"
#include "PlayerComponent.h"
#include <GatesEngine/Header/GameFramework/Collision/CollisionManager.h>


CollisionDetection* CollisionDetection::GetInstance()
{
	static CollisionDetection instance;
	return &instance;

}

void CollisionDetection::Update(float deltaTime)
{
	for (auto& ne : nEnemies)
	{
		if (GE::CollisionManager::CheckHit(player.collider, ne.collider))
		{
			player.object->OnCollision(ne.object);
			ne.object->OnCollision(player.object);
		}
	}
}

void CollisionDetection::SetPlayer(GE::GameObject* p, GE::ICollider* coll)
{
	player.object = p;
	player.collider = coll;
}

void CollisionDetection::SetNormalEnemies(GE::GameObject* ne, GE::ICollider* coll)
{
	nEnemies.push_back({ ne,coll });
}

