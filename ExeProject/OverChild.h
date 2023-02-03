#pragma once
#include<GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Graphics/SkinMeshAnimator.h>

class OverChild:public GE::Component
{
private:
	float gravity;
	GE::SkinMeshAnimator animator;
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
};

