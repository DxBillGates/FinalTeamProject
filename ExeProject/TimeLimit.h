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
	
public:

	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;

};

class TimeLimit 
{
private:
	int timer = 1;//��������
	int minutes = 0;//����
	int tenSeconds = 0;//�b���̏\�̈�
	int oneSeconds = 0;//�b���̈�̈�
	int fps = 144;//�t���[�����[�g


public:
	std::string tag;
public:
	TimeLimit(const int& timer);

	void Start(GE::GameObjectManager* gameObjectManager);
	void Update();

	//�e�N�X�`������
	void Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager,float shift);
};

