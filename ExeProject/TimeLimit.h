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
	int timer = 1;//制限時間
	int minutes = 0;//分数
	int tenSeconds = 0;//秒数の十の位
	int oneSeconds = 0;//秒数の一の位
	int fps = 144;//フレームレート


public:
	std::string tag;
public:
	TimeLimit(const int& timer);

	void Start();
	void Update();
};

