#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class StartTree : public GE::Component
{
private:
	static int goalCollect;			//収集物の目標個数
public:
	static int collectCount;		//持って帰ってきた収集物の個数
	static bool isCollect;			//指定個数集めたかどうか	
	static GE::Math::Vector3 position;
	StartTree();
	void Awake();
	void Start();
	void Update(float deltaTime);
	void Draw();
	void LateDraw();
	void OnCollision(GE::GameObject* other);
	void OnCollision(GE::ICollider* hitCollider);
	void OnGui();
};