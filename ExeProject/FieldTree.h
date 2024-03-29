#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include"FieldObjectDeBugTransform.h"
class FieldTree : public GE::Component
{
public:
	GE::Color body_Color = { 1.0f,0.5f,0.1f,1.0f };
	GE::Color leaf_Color = { 0.1f,1.0f,0.4f,1.0f };

	FieldTree();
	void Awake();
	void Start();
	void Update(float deltaTime);
	void DrawShadow() override;
	void Draw();
	void TreeLeafDraw();
	void LateDraw();
	void OnCollision(GE::GameObject* other);
	void OnCollision(GE::ICollider* hitCollider);
	void OnGui();
	void UnLoad();
};