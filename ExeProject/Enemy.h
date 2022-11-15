#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header\GameFramework/GameSetting.h>
#include"PlayerComponent.h"

class Enemy : public GE::Component
{
public:
	enum class Statas
	{
		ALIVE,
		DEAD,
	};
	Statas statas = Statas::ALIVE;

protected:
	float speed = 0.0f;
	GE::Math::Vector3 bodyDirection = {};
	GE::Material material;
public:
	Enemy();

	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
	virtual void LateDraw() override;
	virtual void OnCollision(GE::GameObject* other) override;
	virtual void OnCollision(GE::ICollider* hitCollider) override;
	virtual void OnGui() override;

};