#include "CollisionDetection.h"
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>

CollisionDetection* CollisionDetection::instance;
CollisionDetection* CollisionDetection::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CollisionDetection();
	}

	return instance;
}

void CollisionDetection::Start()
{
	
}

