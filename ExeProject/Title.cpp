#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >

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

	//�I������
	states = States::start;
	stage = Stage::stage1;

	alpha = 0.0f;
	decided = false;
	option.Awake(gameObjectManager);

	isActive = false;

}

void Title::Update(GE::AudioManager* audioManager)
{
	if (!isActive)return;
	//�I�v�V����������
	option.Update(audioManager);
}

void Title::Select()
{
	if (!isActive)return;
	if (decided) {
		return;
	}

	int a;
	//�㉺�I��(�X�^�[�g,�I�v�V�����Aexit
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::UP)
		|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::W)
		|| inputDevice->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::UP))
	{
		a = ((int)states + ((int)States::serectNum - 1)) % (int)States::serectNum;
		states = (Title::States)a;
	}
	else if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::DOWN)
		|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::S)
		|| inputDevice->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::DOWN))
	{
		a = ((int)states + 1) % (int)States::serectNum;
		states = (Title::States)a;
	}

	////���E�I��(�X�e�[�W
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

	//����
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE)
		|| inputDevice->GetJoyconR()->GetTriggerButton(GE::JoyconButtonData::B))
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
			states = States::start;
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