#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class Ground : public GE::Component
{

public:
	GE::Math::Vector3 position;
	Ground();
	void Awake();
	void Start();
	void Update(float deltaTime);
	void Draw();
	void LateDraw();
	void OnCollision(GE::GameObject* other);
	void OnCollision(GE::ICollider* hitCollider);
	void OnGui();
};