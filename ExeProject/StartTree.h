#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class StartTree : public GE::Component
{
public:
	static int goalCollect;			//���W���̖ڕW��
	static int collectCount;		//�����ċA���Ă������W���̌�
	static bool isCollect;			//�w����W�߂����ǂ���	
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