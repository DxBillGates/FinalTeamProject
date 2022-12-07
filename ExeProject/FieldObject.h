#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class FieldObject : public GE::Component
{

public:
	std::string modelName="Cube";
	std::string shaderName="DefaultMeshShader";
	FieldObject();
	void Awake();
	void Start();
	void Update(float deltaTime);
	void Draw();
	void LateDraw();
	void OnCollision(GE::GameObject* other);
	void OnCollision(GE::ICollider* hitCollider);
	void OnGui();
};