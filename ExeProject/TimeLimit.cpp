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
		minutes = (int)time / 60;//�����̌v�Z
		tenSeconds = (int)time % 60 / 10;//�b���̌v�Z(�\�̈�)
		oneSeconds = (int)time % 60 % 10;//�b���̌v�Z(��̈�)
	}

	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE
		|| PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE_MENU
		|| !isActive)return;

	//���o�̌v�Z
	angle += addAngle;
	tScale = { sinf(angle) * range, sinf(angle) * range, 0.0f };

	//�������Ԃ�30�b�ȉ��ɂȂ�����
	if (minutes == 0 && (int)time % 60 <= 30)
	{
		limit = true;
		interval++;
		//10�b�Ɉ��Ȃ�
		if ((int)time % 60 > 0 && (int)time % 60 % 10 == 0)
		{
			audioManager->Use("hine1")->Start();//��
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