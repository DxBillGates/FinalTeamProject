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
	isActive = false;
}

void TimeLimit::Update()
{


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

	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE
		|| PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE_MENU
		|| !isActive)return;

	time -= GE::GameSetting::Time::GetDeltaTime();
}

void TimeLimit::AddSeconds(const int& seconds)
{
	time += seconds;
}