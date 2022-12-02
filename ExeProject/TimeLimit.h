#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 

class TimeTex : public GE::Component
{
private:
	
public:

	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;

};

class TimeLimit :public GE::Component
{
private:
	int timer = 1;//��������
	int minutes = 0;//����
	int tenSeconds = 0;//�b���̏\�̈�
	int oneSeconds = 0;//�b���̈�̈�
	int fps = 144;//�t���[�����[�g


public:
	std::string tag;
public:
	TimeLimit(const int& timer);

	void Start();
	void Update();
};

