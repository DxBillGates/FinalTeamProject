#pragma once
#include <GatesEngine/Header/GameFramework/Scene/Scene.h>
#include <GatesEngine/Header/GameFramework/Collision/ICollider.h>
#include <GatesEngine/Header/GameFramework/Component/MeshCollider.h>

class SampleScene : public GE::Scene
{
private:
	GE::ICollider* col1, * col2;
	GE::MeshCollider meshCollider;
public:
	SampleScene();
	SampleScene(const std::string& sceneName);
	SampleScene(const std::string& sceneName, const GE::SceneInitializer& initializer);
	~SampleScene();
	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void LateDraw() override;
	void Load() override;
	void UnLoad() override;
};
