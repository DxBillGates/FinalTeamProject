#pragma once
#include<GatesEngine/Header/Graphics/Camera3DDebug.h>
#include <GatesEngine/Header/GameFramework/Component/Component.h>

//�J���������S������N���X
class CameraControl
{
public:
	//static CameraControl* GetInstance();
	void Create();
	void Initialize();	//������
	void Update();		//�X�V

	void Distance_current(float value) { current_cameraDistance = value; }	//�J�����Ƃ̋���
	void Distance_normal(float value) { normal_cameraDistance = value; }	//���i�̋���
	void Distance_dash(float value) { dash_cameraDistance = value; }		//�_�b�V�����̋���
	void SetDir(float value) { dir = value; }								//����
	void SetGraphicsDevice(GE::IGraphicsDeviceDx12* graphicsDevice);		//�擾
	void SetOtherPos(GE::Math::Vector3& pos);								//�J�����������鑊��
	void SetOtherAxis(GE::Math::Axis axis);

	void Direction(GE::Math::Vector3& target);								//�����v�Z
	void DashCam(float dashEasingCount, float dash_time = 100);				//�_�b�V�����v�Z

private:
	//EaseIn�֌W���悭�킩��Ȃ���������ꎞ�I�ɒǉ�
	const float easeIn(const float start, const float end, float time);
private:
	GE::IGraphicsDeviceDx12* graphicsDevice = nullptr;

	GE::Math::Vector3 direction;	//����
	GE::Math::Vector3 position;		//�J�����ʒu
	GE::Math::Vector3 other;		//����
	GE::Math::Axis otherAxis;		//�����Axis

	float dir;					//�Ǐ]����J������Player���猩���p�x
	float current_cameraDistance;//�J������Player�̋���
	float normal_cameraDistance;//�ʏ펞�J������Player�̋���
	float dash_cameraDistance;	//�_�b�V�����J������Player�̋���
};

