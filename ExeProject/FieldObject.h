#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class FieldObjectComponent : public GE::Component
{

public:
	std::string modelName = "Cube";
	std::string shaderName = "DefaultMeshShader";
	std::string textureName = "texture_null";

	float scaleXYZ;
	FieldObjectComponent();
	void Awake();
	void Start();
	void Update(float deltaTime);
	void DrawShadow() override;
	void Draw();
	void LateDraw();
	void OnCollision(GE::GameObject* other);
	void OnCollision(GE::ICollider* hitCollider);
	void OnGui();
};

//class FieldObject
//{
//private:
//	GE::IGraphicsDeviceDx12* graphicsDevice;
//public:
//
//	std::vector<GE::GameObject*> objects;
//public:
//	static FieldObject* GetInstance();
//	void Update();
//	void Draw();
//	void UnLoad();
//
//	void SetGraphicsDevice(GE::IGraphicsDeviceDx12* gDevice);
//
//	void AddTarget(GE::GameObject* gameobject);
//private:
//	FieldObject() = default;
//
//	~FieldObject() = default;
//	void operator=(const FieldObject& obj) = delete;
//	FieldObject(const FieldObject& obj) = delete;
//};