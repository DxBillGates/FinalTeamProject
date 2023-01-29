#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >

#include"Enemy.h"
Enemy::Enemy()
{

}

void Enemy::Awake()
{
}

void Enemy::Start()
{
}

void Enemy::Update(float deltaTime)
{
}

void Enemy::Draw()
{
}

void Enemy::LateDraw()
{
}

void Enemy::OnCollision(GE::GameObject* other)
{

}

void Enemy::OnCollision(GE::ICollider* hitCollider)
{
}

void Enemy::OnCollisionEnter(GE::GameObject* other)
{
	if (statas == Statas::ALIVE)
	{
		if (other->GetTag() == "player")
		{
			statas = Statas::DEAD;
			gameObject->Destroy();
		}
	}
}

void Enemy::OnGui()
{
}

