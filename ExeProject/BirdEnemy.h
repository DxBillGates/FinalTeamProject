#pragma once
#include"Enemy.h"
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class BirdEnemy : public Enemy
{

private:
	float random;
	float angle = 0.0f;
public:
	BirdEnemy();

	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void LateDraw() override;
	void OnCollision(GE::GameObject* other) override;
	void OnCollision(GE::ICollider* hitCollider) override;
	void OnCollisionEnter(GE::GameObject* other)override;

	void OnGui() override;
};