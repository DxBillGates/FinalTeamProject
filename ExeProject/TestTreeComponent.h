#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class TestTreeComponent : public GE::Component
{
public:
	TestTreeComponent();

	void Awake();
	void Start();
	void Update(float deltaTime);
	void Draw();
	void LateDraw();
	void OnCollision(GE::GameObject* other);
	void OnCollision(GE::ICollider* hitCollider);
	void OnGui();
};