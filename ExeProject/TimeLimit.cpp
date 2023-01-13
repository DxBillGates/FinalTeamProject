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
	const int DEFAULT_TIME = 3;//��������(���w��)
	time = DEFAULT_TIME * 60;
	minutes = tenSeconds = oneSeconds = 0; // �e���Ԓl������
	timeOver = false;//�^�C���I�[�o�[�t���O
}

void TimeLimit::Update()
{

	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE)return;

	if (time <= 0)
	{
		timeOver = true;
	}
	else
	{
		minutes = (int)time / 60;//�����̌v�Z
		tenSeconds = (int)time % 60 / 10;//�b���̌v�Z(�\�̈�)
		oneSeconds = (int)time % 60 % 10;//�b���̌v�Z(��̈�)
	}

	time -= GE::GameSetting::Time::GetDeltaTime();
}

void TimeLimit::AddSeconds(const int& seconds)
{
	time += seconds;
}