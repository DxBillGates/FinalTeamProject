#include "TimeLimit.h"

#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include<GatesEngine/Header/Graphics/Texture.h>
#include"PlayerComponent.h"
#include <GatesEngine/Header/GameFramework/GameSetting.h>

TimeLimit* TimeLimit::GetInstance()
{
	static TimeLimit instance;
	return &instance;
}

void TimeLimit::Start(GE::GameObjectManager* gameObjectManager)
{
	const int DEFAULT_TIME = 3;//制限時間(分指定)
	time = DEFAULT_TIME * 60;
	minutes = tenSeconds = oneSeconds = 0; // 各時間値初期化
	timeOver = false;//タイムオーバーフラグ
	limit = false;
	isActive = false;

	angle = 0.0f;
	addAngle = 0.075f;
	range = 0.5f;
	tScale = {};
	interval = 0;
}

void TimeLimit::Update(GE::AudioManager* audioManager)
{
	if (time <= 0)
	{
		timeOver = true;
	}
	else
	{
		minutes = (int)time / 60;//分数の計算
		tenSeconds = (int)time % 60 / 10;//秒数の計算(十の位)
		oneSeconds = (int)time % 60 % 10;//秒数の計算(一の位)
	}

	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE
		|| PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE_MENU
		|| !isActive)return;

	//演出の計算
	angle += addAngle;
	tScale = { sinf(angle) * range, sinf(angle) * range, 0.0f };

	//制限時間が30秒以下になったら
	if (minutes == 0 && (int)time % 60 <= 30)
	{
		limit = true;
		interval++;
		//10秒に一回なる
		if ((int)time % 60 > 0 && (int)time % 60 % 10 == 0)
		{
			audioManager->Use("hine1")->Start();//音
		}
	}
	else
	{
		limit = false;
	}

	time -= GE::GameSetting::Time::GetDeltaTime();
}

void TimeLimit::AddSeconds(const int& seconds)
{
	time += seconds;
}