#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include"CameraControl.h"

class PlayerComponent : public GE::Component
{
public:
	enum class PlayerStatas
	{
		STOP_DEBUG,
		MOVE,
		DASH,
		LOCKON_SHOOT,
		STAY_LAND,
	};
	PlayerStatas statas;				//Playerの状態
private:
	GE::InputDevice* inputDevice;
	GE::Math::Vector3 gyro;

	GE::Math::Vector3 body_direction;//体の向き計算用
	float dashEasingCount;			//スピード遷移のカウント

	GE::Math::Vector3 gravity;	//重力
	float dir;					//追従するカメラのPlayerから見た角度
	float current_speed;		//現在のスピード
	float normal_speed;			//通常時のスピード
	float dash_speed;			//ダッシュ時のスピード


	bool isLockOn;				//ロックオンして発射待機中フラグ

	struct LockOnEnemy
	{
		GE::GameObject* object = nullptr;
		GE::Math::Vector3 direction;
	};
	LockOnEnemy lockOnEnemy;

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
	void Control(float deltaTime, float gameTime);
	void LockOn();
	void Dash(float dash_time, float deltaTime, float gameTime);
	//EaseIn関係がよくわからなかったから一時的に追加
	const float easeIn(const float start, const float end, float time);
};