#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/Graphics/SkinMeshAnimator.h>
#include <GatesEngine/Header/Audio/AudioManager.h>

class BirdChild : public GE::Component
{
private:

	GE::AudioManager* audioManager;
	GE::SkinMeshAnimator animator;
	bool isJump;
	int jumpCount;
	float count;
public:

public:
	BirdChild();
	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void DrawShadow() override;
	void Draw() override;
	void LateDraw() override;
	void OnCollision(GE::GameObject* other) override;
	void OnCollisionEnter(GE::GameObject* other)override;
	void OnCollisionExit(GE::GameObject* other)override;
	void OnCollision(GE::ICollider* hitCollider) override;
	void OnGui() override;
};