#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include<GatesEngine/Header/Audio/AudioManager.h>


class OptionData
{
public:
	static int BGM_vol;
	static int SE_vol;
};

class Option
{
public:
	//�I��
	enum class Select
	{
		BGM_VOL,
		SE_VOL,
		Back,

		SelectNum,
		volR,
		volL,
	};
private:

	bool bgmChange = false;
	bool seChange = false;
public:
	static int Max_vol;
	//�I��
	static 	Select select;

private:
	//�e�N�X�`������
	void Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager, GE::Math::Vector2 pos, float scaleX, Select handle);
	void KeySelect();
	void Back();
public:
	//�����ݒ�
	void Awake(GE::GameObjectManager* gameObjectManager);
	void Update(GE::AudioManager* audioManager);
};