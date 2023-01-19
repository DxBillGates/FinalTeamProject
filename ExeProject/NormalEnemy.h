#pragma once
#include"Enemy.h"
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class NormalEnemy : public Enemy
{
	
private:
	GE::Math::Vector3 random;
	float angle = 0.0f;
	GE::SkinMeshAnimator animator;

public:
	NormalEnemy();

	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void DrawShadow() override;
	void Draw() override;
	void LateDraw() override;
	void OnCollision(GE::GameObject* other) override;
	void OnCollision(GE::ICollider* hitCollider) override;
	void OnGui() override;
};