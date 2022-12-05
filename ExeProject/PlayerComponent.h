#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/Graphics/SkinMeshAnimator.h>

class PlayerComponent : public GE::Component
{
public:
	static float current_speed;		//���݂̃X�s�[�h
	static float normal_speed;		//�ʏ펞�̃X�s�[�h
	bool isCollect;					//�w����W�߂����ǂ���	
private:
	static GE::Math::Vector3 gravity;	//�d��
	static float rayHitSecond;		//�Ə������킹�钷���i�b���j
	static int hitStopTime;
	static float damageSpeed;
	static float pushStartTime;		//�L�[�������Ă��瑀��ł���悤�ɂȂ�܂ł̃J�E���g
	static float stayLandLerpTime;	//�؂ɖ߂郉�[�v
	static int goalCollect;			//���W���̖ڕW��
	static int collectMax;			//���W���̓����ɂ��Ă�ő��
	static float body_direction_LerpTime; //�b��

	GE::InputDevice* inputDevice;
	GE::Math::Vector3 gyro;
	GE::Math::Vector3 accelerometer;
	GE::SkinMeshAnimator animator;

	bool isLockOnStart;				//���b�N�I���������Ăԃt���O
	bool isLockOn;					//���b�N�I�����Ĕ��ˑҋ@���t���O

	float dashEasingCount;			//�X�s�[�h�J�ڂ̃J�E���g
	float stayLandLerpEasingCount;	//��������J�ڃJ�E���g
	float rayHitCount;				//���t���[���Ə������킹�Ă��邩
	int collectCount;				//��W�������W�߂���
	int hitStopCount;				//�q�b�g�X�g�b�v�J�E���g�p
	float startCouunt;				//�J�n���̃J�E���g
	int body_direction_LerpCount;	//���̎p���ɖ߂�Ƃ��̑J��
	struct LockOnEnemy
	{
		GE::GameObject* object = nullptr;
		GE::Math::Vector3 direction;
	};
	LockOnEnemy lockOnEnemy;
	
	//���e�B�N���̈ʒu
	GE::Math::Vector2 center;
	
	GE::Math::Quaternion quat;
	GE::Math::Vector3 body_direction;				//�̂̌����v�Z�p
	GE::Math::Quaternion body_direction_LockOn;		//���b�N�I�����̑̂̌����v�Z�p
	bool is_rayCast_active;							//���C�L���X�g�̏Ə����g���Ă邩

	GE::Math::Vector3 currentPosition;//���ɒ�������Ƃ��̃��[�v�p
public:
	enum class PlayerStatas
	{
		STOP_DEBUG,
		MOVE,
		DASH,
		LOCKON_SHOOT,
		GO_TREE,
		STAY_TREE,
	};
	PlayerStatas statas;				//Player�̏��
	//RayCast�p
	GE::Math::Vector3 rayPos, rayDir;

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
	/// <summary>
	/// ���ȏ�̃X�s�[�h�ɂȂ��Ă��邩���擾���邽��
	/// </summary>
	/// <returns></returns>
	static bool IsSpeedy();
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
	/// ���W���̐���
	/// </summary>
	void CollectControl();
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
	/// <param name="loop">True�̊Ԃ̓_�b�V����������BFalse�ɂȂ��dash_time�̃t���[���������Č�������B�ŏ�����false����dash_time�t���[������������</param>
	void Dash(float dash_speed, float dash_time, float deltaTime, GE::Math::Vector3 direction, bool loop = false);
	/// <summary>
	/// 
	/// </summary>
	void RayCast(float deltaTime);
	//EaseIn�֌W���悭�킩��Ȃ���������ꎞ�I�ɒǉ�
	const float easeIn(const float start, const float end, float time);
};