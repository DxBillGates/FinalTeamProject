#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include "InputManager.h"

#include "Title.h"
#include"PlayerComponent.h"

Title* Title::GetInstance()
{
	static Title instance;
	return &instance;
}

void Title::Awake(GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device)
{
	GE::Utility::Printf("Title Awake()\n");

	inputDevice = GE::InputDevice::GetInstance();

	//選択項目
	states = states;
	stage = Stage::stage1;

	alpha = 0.0f;
	decided = false;
	option.Awake(gameObjectManager);

	isExit = false;
	isActive = false;

}

void Title::Update(GE::AudioManager* audioManager)
{
	if (!isActive)return;

	//オプション中処理
	option.Update(audioManager);
}

void Title::Select()
{
	if (!isActive)return;

	if (decided) {
		return;
	}

	int a;
	auto inputManager = InputManager::GetInstance();
	//上下選択(スタート,オプション、exit
	if (inputManager->GetTriggerDirection().y >= 1)
	{
		a = ((int)states + ((int)States::serectNum - 1)) % (int)States::serectNum;
		states = (Title::States)a;
	}
	else if (inputManager->GetTriggerDirection().y <= -1)
	{
		a = ((int)states + 1) % (int)States::serectNum;
		states = (Title::States)a;
	}

	////左右選択(ステージ
	//if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::RIGHT)
	//	|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D)
	//	|| inputDevice->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::RIGHT))
	//{
	//	a = (stage + (Stage::stageNum - 1)) % Stage::stageNum;
	//	stage = (Title::Stage)a;
	//	sprites[Title::States::start]->SetTexture(textures[stage]);
	//}
	//else if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::LEFT)
	//	|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::A)
	//	|| inputDevice->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::LEFT))
	//{
	//	a = (stage + 1) % Stage::stageNum;
	//	stage = (Title::Stage)a;
	//	sprites[Title::States::start]->SetTexture(textures[stage]);
	//}

	//決定
	if (inputManager->GetActionButton())
	{
		switch (states)
		{
		case Title::States::start:
			switch (stage)
			{
			case Title::Stage::stage1:
				GE::Utility::Printf("Title stage1\n");
				break;
			case Title::Stage::stage2:
				GE::Utility::Printf("Title stage2\n");
				break;
			default:
				break;
			}
			break;
		case Title::States::option:
			GE::Utility::Printf("Title option()\n");
			break;
		case Title::States::exit:
			GE::Utility::Printf("Title exit()\n");
			//states = States::start;
			isExit = true;
			break;
		default:
			break;
		}
		decided = true;
	}
}

bool Title::GetSelect(Title::States s)
{
	if (decided)
	{
		if (s == states)
		{
			return true;
		}
	}
	return false;
}

void Title::ClearGameObject()
{
}