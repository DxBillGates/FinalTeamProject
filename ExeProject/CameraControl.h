#pragma once
#include<GatesEngine/Header/Graphics/Camera3DDebug.h>
#include <GatesEngine/Header/GameFramework/Component/Component.h>

//�J���������S������N���X
class CameraControl
{
private:
	GE::IGraphicsDeviceDx12* graphicsDevice = nullptr;

	GE::Math::Vector3 position;		//�J�����ʒu
	GE::Math::Vector3 target;		//�J�����������W
	GE::GameObject* targetObject;

	float dir;

	float current_cameraDistance;//�J������Player�̋���
	float normal_cameraDistance;//�ʏ펞�J������Player�̋���
	float dash_cameraDistance;	//�_�b�V�����J������Player�̋���

	//�J�����V�F�C�N�p
	int shakeFlame;
	GE::Math::Vector2 range;
	GE::Math::Vector3 cameraShake;
	//�J�����̃u���p
	float count;

public:
	static CameraControl* GetInstance();

	void Create();
	void Initialize();	//������
	void Update(float deltaTime);		//�X�V

	void Distance_current(float value) { current_cameraDistance = value; }	//�J�����Ƃ̋���
	void Distance_normal(float value) { normal_cameraDistance = value; }	//���i�̋���
	void Distance_dash(float value) { dash_cameraDistance = value; }		//�_�b�V�����̋���
	void SetGraphicsDevice(GE::IGraphicsDeviceDx12* graphicsDevice);		//�擾

	/// <summary>
	/// �J�����V�F�C�N�J�n���ݒ�
	/// </summary>
	/// <param name="range">�h�炷�͈�</param>
	/// <param name="flame">�h�炷�t���[����</param>
	void ShakeStart(GE::Math::Vector2 range = { 10,10 }, int flame = 5);
	void SetTargetObject(GE::GameObject* t) { targetObject = t; }
	void SetClearCameraPosition();
	void SetOverCameraPosition();

private:
	CameraControl() = default;
	~CameraControl() = default;

	// ����A�R�s�[�̋֎~
	void operator=(const CameraControl & obj) = delete;
	CameraControl(const CameraControl & obj) = delete;

	void Shake(float deltaTime);
	//EaseIn�֌W���悭�킩��Ȃ���������ꎞ�I�ɒǉ�
	const float easeIn(const float start, const float end, float time);
};

