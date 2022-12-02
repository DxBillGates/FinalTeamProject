#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 

class TimeLimit
{
private:
	int timer = 1;//制限時間
	int minutes = 0;//分数
	int tenSeconds = 0;//秒数の十の位
	int oneSeconds = 0;//秒数の一の位
	int fps = 144;//フレームレート


public:
	TimeLimit(const int& timer);

	void Start();
	void Update();
	void Draw();
};

