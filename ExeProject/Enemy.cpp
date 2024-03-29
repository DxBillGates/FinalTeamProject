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
	restInterval = 2500.f;
	restCount = restInterval;
}

void Enemy::Update(float deltaTime)
{
	const float f = 144.0f / (1.0f / deltaTime);

	if (statas == Statas::REST)
	{
		if (PlayerComponent::dashMode)
		{
			if (restCount < restInterval)
			{
				restCount += f * GE::GameSetting::Time::GetGameTime();
			}
			else
			{
				//statas = Statas::ALIVE;
			}
		}
		else
		{
			if (PlayerComponent::statas == PlayerComponent::PlayerStatas::STAY_TREE)
			{
				statas = Statas::ALIVE;
				transform->position = startPosition;
			}
		}
	}

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
			statas = Statas::REST;
			restCount = 0.0f;
		}
	}
}

void Enemy::OnGui()
{
}

