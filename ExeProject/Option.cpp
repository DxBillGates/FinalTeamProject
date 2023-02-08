#include "Option.h"
#include"Title.h"
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include<GatesEngine/Header/Graphics/Texture.h>
#include "InputManager.h"

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
	auto inputManager = InputManager::GetInstance();
	int a;
	//変更項目選択
	if (inputManager->GetTriggerDirection().y >= 1)
	{
		a = ((int)select + ((int)Select::SelectNum - 1)) % (int)Select::SelectNum;
		select = (Select)a;
	}
	else if (inputManager->GetTriggerDirection().y <= -1)
	{
		a = ((int)select + 1) % (int)Select::SelectNum;
		select = (Select)a;
	}

	//内容
	if (inputManager->GetTriggerDirection().x <= -1)
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
	else if (inputManager->GetTriggerDirection().x >= 1)
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
	auto inputManager = InputManager::GetInstance();
	if (select == Select::Back)
	{
		if (inputManager->GetActionButton())

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