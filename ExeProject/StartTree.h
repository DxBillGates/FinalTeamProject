#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class StartTree : public GE::Component
{
public:
	static int goalCollect;			//収集物の目標個数
	static int collectCount;		//持って帰ってきた収集物の個数
	static bool isCollect;			//指定個数集めたかどうか	
	GE::Color body_Color = { 0.7f,0.3f,0.1f,1.0f };
	GE::Color leaf_Color = { 1.0f,1.0f,1.0f,1.0f };
	GE::Math::Vector3 rotation_euler;

	StartTree();
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
};