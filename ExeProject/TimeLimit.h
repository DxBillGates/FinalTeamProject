#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 

class TimeTex : public GE::Component
{
public:
	GE::Math::Vector3 position;
	GE::Math::Vector3 scale;
	GE::Math::Quaternion rotation;
	std::string tag;

	float pivotPosX;
	float texSizeX;

	enum class TimeName
	{
		minutes,
		tenSeconds,
		oneSeconds,
	};

	int num = 0;

public:

	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;

};

class TimeLimit 
{
private:
	float time;
	int minutes = 0;//�����̌v�Z�ϐ�
	int tenSeconds = 0;//�b���̌v�Z�ϐ�(�\�̈�)
	int oneSeconds = 0;//�b���̌v�Z�ϐ�(��̈�)
	bool timeOver = false;//�^�C���I�[�o�[�t���O

public:
	std::string tag;
public:
	static TimeLimit* GetInstance();
	void Start(GE::GameObjectManager* gameObjectManager);
	void Update();
	void AddSeconds(const int& seconds);//���Ԓǉ��������Ƃ��ɌĂԊ֐�

	const int& GetMinutes() { return minutes; }//�����̃Q�b�g�֐�
	const int& GetTenSeconds() { return tenSeconds; }//�b���̃Q�b�g�֐�(�\�̈�)
	const int& GetOneSeconds() { return oneSeconds; }//�b���̃Q�b�g�֐�(��̈�)
	const bool& GetTimeOver() { return timeOver; }//�^�C���I�[�o�[�t���O�̃Q�b�g�֐�

	//�e�N�X�`������
	void Create(const std::string& gui_tag, const std::string& tex_tag, GE::GameObjectManager* gameObjectManager, float posX, float scaleX, int timeNum);
private:
	TimeLimit() = default;
	~TimeLimit() = default;

	// ����A�R�s�[�̋֎~
	void operator=(const TimeLimit& obj) = delete;
	TimeLimit(const TimeLimit& obj) = delete;
};

