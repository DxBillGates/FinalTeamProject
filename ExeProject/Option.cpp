#include "Option.h"
#include"Title.h"
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include<GatesEngine/Header/Graphics/Texture.h>

int OptionData::BGM_vol = 1;
int OptionData::SE_vol = 3;
int Option::Max_vol = 5;
Option::Select Option::select;

void Option::Awake(GE::GameObjectManager* gameObjectManager)
{
	select = Select::BGM_VOL;
	bgmChange = false;
	seChange = false;
}

void Option::KeySelect()
{
	int a;
	//変更項目選択
	if (GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::UP)
		|| GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::W)
		|| GE::InputDevice::GetInstance()->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::UP))
	{
		a = ((int)select + ((int)Select::SelectNum - 1)) % (int)Select::SelectNum;
		select = (Select)a;
	}
	else if (GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::DOWN)
		|| GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::S)
		|| GE::InputDevice::GetInstance()->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::DOWN))
	{
		a = ((int)select + 1) % (int)Select::SelectNum;
		select = (Select)a;
	}

	//内容
	if (GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::LEFT)
		|| GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::A)
		|| GE::InputDevice::GetInstance()->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::LEFT))
	{
		if (select == Select::BGM_VOL)
		{
			if (OptionData::BGM_vol > 0)
			{
				bgmChange = true;
				OptionData::BGM_vol--;
			}
		}
		else if (select == Select::SE_VOL)
		{
			if (OptionData::SE_vol > 0)
			{
				seChange = true;
				OptionData::SE_vol--;
			}
		}
	}
	else if (GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::RIGHT)
		|| GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::D)
		|| GE::InputDevice::GetInstance()->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::LEFT))
	{
		if (select == Select::BGM_VOL)
		{
			if (OptionData::BGM_vol < Max_vol)
			{
				OptionData::BGM_vol++;
				bgmChange = true;
			}
		}
		else if (select == Select::SE_VOL)
		{
			if (OptionData::SE_vol < Max_vol)
			{
				OptionData::SE_vol++;
				seChange = true;
			}
		}
	}

	Back();
}

void Option::Back()
{
	if (select == Select::Back)
	{
		if (GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE)
			|| GE::InputDevice::GetInstance()->GetJoyconR()->GetTriggerButton(GE::JoyconButtonData::B))

		{
			Title::GetInstance()->Back();
			select = Select::BGM_VOL;
		}
	}
}

void Option::Update(GE::AudioManager* audioManager)
{
	if (!Title::GetInstance()->GetSelect(Title::States::option))return;

	KeySelect();

	//音量調整をしたなら
	if (bgmChange)
	{
		float set = OptionData::BGM_vol / 10.0f;
		//変更項目
		audioManager->Get("testBGM", 0)->SetVolume(set);
		bgmChange = false;
	}
	if (seChange)
	{
		float set = OptionData::SE_vol / 10.0f;
		//変更項目
		audioManager->Get("hitWall", 0)->SetVolume(set);
		audioManager->Get("hitWall", 1)->SetVolume(set);
		audioManager->Get("air1", 0)->SetVolume(set);
		audioManager->Get("catch2", 0)->SetVolume(set);
		audioManager->Get("catch2", 1)->SetVolume(set);
		audioManager->Get("catch2", 2)->SetVolume(set);
		audioManager->Get("flapping1", 0)->SetVolume(set);
		audioManager->Get("flapping1", 1)->SetVolume(set);
		seChange = false;
	}
}