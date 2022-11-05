#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class NormalEnemy : public GE::Component
{
private:
	GE::Math::Vector3 random;
	float speed = 0.0f;
	float angle = 0.0f;
public:
	NormalEnemy();

	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void LateDraw() override;
	void OnCollision(GE::GameObject* other) override;
	void OnCollision(GE::ICollider* hitCollider) override;
	void OnGui() override;
};