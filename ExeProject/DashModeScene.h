#pragma once
#include <GatesEngine/Header/GameFramework/Scene/Scene.h>
#include <GatesEngine/Header/GameFramework/Collision/ICollider.h>
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>


class DashModeScene :public GE::Scene
{
private:
	GE::DirectionalLight* directionalLight;
public:
	DashModeScene();
	DashModeScene(const std::string& sceneName);
	DashModeScene(const std::string& sceneName, const GE::SceneInitializer& initializer);
	~DashModeScene();
	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void LateDraw() override;
	void Load() override;
	void UnLoad() override;
};

