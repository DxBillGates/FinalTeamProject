#pragma once
#include"Option.h"

#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include<GatesEngine/Header/Audio/AudioManager.h>

//�^�C�g���N���X
class Title
{
private:
	GE::InputDevice* inputDevice;
	
	bool decided = false;
	Option option;

public:
	//�I������
	enum class States
	{
		start,
		endless,
		option,
		exit,

		serectNum,
	};
	//�I���X�e�[�W
	enum class Stage
	{
		stage1,
		stage2,

		stageNum,
	};

	States states = States::start;
	Stage stage;
	bool isActive;
	float alpha = 0.0f;
	bool isExit;
private:
	Title() = default;
	~Title() = default;
	void operator=(const Title & obj) = delete;
	Title(const Title & obj) = delete;

	//�e�N�X�`������
	void Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device);
public:
	static Title* GetInstance();

	//�I��
	void Select();

	//�����ݒ�
	void Awake(GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device);
	void Update(GE::AudioManager* audioManager);

	bool GetDecid() { return decided; }

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="s">�w����e</param>
	/// <returns>�w����e���^�C�g���őI�����ꂽ��</returns>
	bool GetSelect(Title::States s);

	void Back() { decided = false; }

	void ClearGameObject();
};

