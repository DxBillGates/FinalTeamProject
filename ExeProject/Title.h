#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include<GatesEngine/Header/Graphics/Texture.h>

//�^�C�g���p�e�N�X�`���N���X
class TitleTex :public GE::Component
{
private:
	GE::GameObject* targetObject = nullptr;
public:
	GE::Math::Vector3 position;
	GE::Math::Vector3 scale;
	std::string tag;
public:
	void Awake();
	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;

	void SetTargetObject(GE::GameObject* t) { targetObject = t; }
private:
};

//�^�C�g���N���X
class Title
{
private:
	GE::GameObject* targetObject = nullptr;

	GE::InputDevice* inputDevice;

	std::vector<GE::GameObject*> sprites;
public:
	//�I������
	enum States
	{
		start,
		option,
		exit,

		serectNum,
	};
	//�I���X�e�[�W
	enum Stage
	{
		stage1,
		stage2,

		stageNum,
	};

	States states;
	Stage stage;

private:
	//�I��
	void Serect();
	//�e�N�X�`������
	void Create(std::string gui_tag,std::string tex_tag ,GE::GameObjectManager* gameObjectManager, GE::GameObject* t);
public:
	static Title* GetInstance();
	Title() = default;
	~Title() = default;
	void operator=(const Title& obj) = delete;
	Title(const Title& obj) = delete;

	//�����ݒ�
	void Start(GE::GameObjectManager* gameObjectManager, GE::GameObject* t);
	void Update();
};

