#pragma once
#include <GatesEngine/Header/GameFramework/Scene/Scene.h>
#include <GatesEngine/Header/GameFramework/Collision/ICollider.h>
#include <GatesEngine/Header/GameFramework/Component/MeshCollider.h>
#include <GatesEngine/Header/Graphics/MeshData.h>
#include <GatesEngine/Header/Graphics/VertexData.h>
#include <GatesEngine/Header/Graphics/MeshCreater.h>
#include <GatesEngine/Header/Graphics/Mesh.h>

class SampleScene : public GE::Scene
{
private:
	GE::ICollider* col1, * col2;
	GE::MeshData<GE::Vertex_UV_Normal> groundModel;
	GE::MeshData<GE::Vertex_UV_Normal> startTreeModel;
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
