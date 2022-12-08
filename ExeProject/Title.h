#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 

//�^�C�g���p�e�N�X�`���N���X
class TitleTex :public GE::Component
{
private:
	GE::ITexture* tex;
public:
	GE::Math::Vector3 position = {};
	GE::Math::Vector3 scale = {};
	GE::Math::Quaternion rotation = {};
	std::string tag;
public:
	void Awake();
	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;

	void SetTexture(GE::ITexture* setTex) { tex = setTex; }
};

//�^�C�g���N���X
class Title
{
private:
	GE::InputDevice* inputDevice;

	std::vector<TitleTex*> sprites;
	std::vector<GE::ITexture*> textures;

	bool decided = false;
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

	float alpha = 0.0f;
private:
	//�e�N�X�`������
	void Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device);
public:
	static Title* GetInstance();
	Title() = default;
	~Title() = default;
	void operator=(const Title& obj) = delete;
	Title(const Title& obj) = delete;

	//�I��
	void Select();

	//�����ݒ�
	void Awake(GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device);
	void Update();

	bool GetDecid() { return decided; }

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="s">�w����e</param>
	/// <returns>�w����e���^�C�g���őI�����ꂽ��</returns>
	bool GetSelect(Title::States s);

	void ClearGameObject();
};

