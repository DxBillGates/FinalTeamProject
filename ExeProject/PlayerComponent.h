#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>

class PlayerComponent : public GE::Component
{
private:
	GE::InputDevice* inputDevice;
	GE::Math::Vector3 gyro;
	GE::Math::Vector3 accelerometer;

	GE::Math::Vector3 body_direction;//�̂̌����v�Z�p
	float dashEasingCount;			//�X�s�[�h�J�ڂ̃J�E���g

	GE::Math::Vector3 gravity;	//�d��
	float dir;					//�Ǐ]����J������Player���猩���p�x
	float current_speed;		//���݂̃X�s�[�h
	float normal_speed;			//�ʏ펞�̃X�s�[�h

	bool isLockOnStart;			//���b�N�I���������Ăԃt���O
	bool isLockOn;				//���b�N�I�����Ĕ��ˑҋ@���t���O
	float rayHitCount;	//���t���[���Ə������킹�Ă��邩
	float rayHitSecond;	//�Ə������킹�钷���i�b���j

	struct LockOnEnemy
	{
		GE::GameObject* object = nullptr;
		GE::Math::Vector3 direction;
	};
	LockOnEnemy lockOnEnemy;

	//�q�b�g�X�g�b�v�J�E���g�p
	int hitStopCount;
	//�q�b�g�X�g�b�v�̒���(�b��)
	int hitStopTime;

	//���e�B�N���̈ʒu
	GE::Math::Vector2 center;

	//���̎p���ɖ߂�Ƃ��̑J��
	int body_direction_LerpCount;
	float body_direction_LerpTime; //�b��

	GE::Math::Quaternion quat;
	GE::Math::Quaternion body_direction_LockOn;

	bool is_rayCast_active;
public:
	enum class PlayerStatas
	{
		STOP_DEBUG,
		MOVE,
		DASH,
		LOCKON_SHOOT,
		STAY_LAND,
	};
	PlayerStatas statas;				//Player�̏��

	static float GameTime;
	//RayCast�p
	GE::Math::Vector3 rayPos, rayDir;

public:
public:
	PlayerComponent();
	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void LateDraw() override;
	void OnCollision(GE::GameObject* other) override;
	void OnCollisionEnter(GE::GameObject* other)override;
	void OnCollisionExit(GE::GameObject* other)override;
	void OnCollision(GE::ICollider* hitCollider) override;
	void OnGui() override;
private:
	/// <summary>
	/// ����֌W
	/// </summary>
	/// <param name="deltaTime">�t���[�����[�g�̒l</param>
	/// <param name="gameTime">�Q�[���̎��Ԃ̑���</param>
	void Control(float deltaTime);
	/// <summary>
	/// Keyboard�ňړ�����
	/// </summary>
	void KeyboardMoveControl();
	/// <summary>
	/// �O���ɂ��čł��߂��G�����߂�
	/// </summary>
	void SearchNearEnemy();
	/// <summary>
	/// ���b�N�I�����čU��
	/// </summary>
	void LockOn();
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="dash_speed">����</param>
	/// <param name="dash_time">�_�b�V���̒����i���ԁj</param>
	/// <param name="deltaTime">�t���[�����[�g�̒l</param>
	/// <param name="gameTime">�Q�[���̎��Ԃ̑���</param>
	/// <param name="direction">����</param>
	void Dash(float dash_speed, float dash_time, float deltaTime, GE::Math::Vector3 direction, bool loop = false);
	/// <summary>
	/// 
	/// </summary>
	void RayCast(float deltaTime);
	//EaseIn�֌W���悭�킩��Ȃ���������ꎞ�I�ɒǉ�
	const float easeIn(const float start, const float end, float time);
};