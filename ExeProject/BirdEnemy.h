#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class BirdEnemy : public GE::Component
{

private:
	GE::Math::Vector3 random;
	float speed = 0.0f;
	float angle = 0.0f;
	GE::Math::Vector3 bodyDirection;
	GE::Material material;
	GE::Math::Vector3 velocity;//‘Ì‚ÌŒü‚«ŒvŽZ—p
public:
	enum class Statas
	{
		ALIVE,
		DEAD,
	};
	Statas statas;
	static float GameTime;

public:
	BirdEnemy();

	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void LateDraw() override;
	void OnCollision(GE::GameObject* other) override;
	void OnCollision(GE::ICollider* hitCollider) override;
	void OnGui() override;

	void SetColor(GE::Color color);

};