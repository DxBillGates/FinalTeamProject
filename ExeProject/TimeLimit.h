#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 

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
	//�J�n���Ă����߂Ĕ�ї��܂ŃJ�E���g���Ȃ��p
	bool isActive;
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

