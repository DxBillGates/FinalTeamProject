#pragma once
#include <GatesEngine/Header/GameFramework/Scene/Scene.h>
#include <GatesEngine/Header/GameFramework/Collision/ICollider.h>
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

class OverTex :public GE::Component
{
public:
	void Awake();
	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;
};

class Over :public GE::Scene
{
public:
	static bool nowOver;
	//カメラのスピード
	static float CameraSpeed; 
private:
		GE::DirectionalLight* directionalLight;

public:
	Over();
	Over(const std::string& sceneName);
	Over(const std::string& sceneName, const GE::SceneInitializer& initializer);
	~Over();
	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void LateDraw() override;
	void Load() override;
	void UnLoad() override;
};

