#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class VolumetricCloud : public GE::Component
{
private:
public:
	void Start() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void LateDraw() override;
};