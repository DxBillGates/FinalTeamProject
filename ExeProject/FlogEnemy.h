#pragma once
#include"Enemy.h"
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class FlogEnemy : public Enemy
{
private:
	float random;
	float angle = 0.0f;

	bool jump;
	int jumpCount;
	GE::SkinMeshAnimator animator;

	GE::Math::Vector3 velocity;//�ړ���
	GE::Math::Vector3 accelerate;//�����x
	GE::Math::Vector3 gravity;//�d��
public:
	FlogEnemy();

	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void DrawShadow()override;
	void Draw() override;
	void LateDraw() override;
	void OnCollision(GE::GameObject* other) override;
	void OnCollision(GE::ICollider* hitCollider) override;
	void OnCollisionEnter(GE::GameObject* other)override;

	void OnGui() override;
};


