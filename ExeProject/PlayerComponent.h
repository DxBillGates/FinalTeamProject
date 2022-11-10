#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include"CameraControl.h"

class PlayerComponent : public GE::Component
{
public:
	enum class PlayerState
	{
		STOP_DEBUG,
		MOVE,
		DASH,
		STAY_LAND,
	};
	PlayerState state;				//Player�̏��
private:
	GE::InputDevice* inputDevice;
	GE::Math::Vector3 random;
	GE::Math::Vector3 gyro;
	GE::Math::Vector3 accelerometer;

	GE::Math::Vector3 body_direction;//�̂̌����v�Z�p
	float dashEasingCount;			//�X�s�[�h�J�ڂ̃J�E���g

	GE::Math::Vector3 gravity;	//�d��
	float dir;					//�Ǐ]����J������Player���猩���p�x
	float current_cameraDistance;//�J������Player�̋���
	float normal_cameraDistance;//�ʏ펞�J������Player�̋���
	float dash_cameraDistance;	//�_�b�V�����J������Player�̋���
	float current_speed;		//���݂̃X�s�[�h
	float normal_speed;			//�ʏ펞�̃X�s�[�h
	float dash_speed;			//�_�b�V�����̃X�s�[�h

	GE::Math::Quaternion quat;
public:
	PlayerComponent();
	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void LateDraw() override;
	void OnCollision(GE::GameObject* other) override;
	void OnCollision(GE::ICollider* hitCollider) override;
	void OnGui() override;
private:
	void Control(float deltaTime);
	bool LockOn();
	//EaseIn�֌W���悭�킩��Ȃ���������ꎞ�I�ɒǉ�
	const float easeIn(const float start, const float end, float time);
};