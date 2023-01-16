#pragma once
#include <GatesEngine/Header/GameFramework/Scene/Scene.h>
#include <GatesEngine/Header/GameFramework/Collision/ICollider.h>
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

class ClearTex :public GE::Component
{
public:
	void Awake();
	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;
};

class Clear : public GE::Scene
{
private:
	GE::DirectionalLight* directionalLight;
public:
	Clear();
	Clear(const std::string& sceneName);
	Clear(const std::string& sceneName, const GE::SceneInitializer& initializer);
	~Clear();
	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void LateDraw() override;
	void Load() override;
	void UnLoad() override;
};

