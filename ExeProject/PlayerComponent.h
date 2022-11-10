#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
class PlayerComponent : public GE::Component
{
private:
	GE::InputDevice* inputDevice;
	GE::Math::Vector3 random;
	GE::Math::Vector3 gyro;
	GE::Math::Vector3 accelerometer;

	GE::Math::Vector3 body_direction;//体の向き計算用
	float dashEasingCount;			//スピード遷移のカウント

	GE::Math::Vector3 gravity;	//重力
	float dir;					//追従するカメラのPlayerから見た角度
	float current_speed;		//現在のスピード
	float normal_speed;			//通常時のスピード

	bool isLockOnStart;			//ロックオン処理を呼ぶフラグ
	bool isLockOn;				//ロックオンして発射待機中フラグ
	float rayHitCount;	//何フレーム照準をあわせているか
	float rayHitSecond;	//照準を合わせる長さ（秒数）

	struct LockOnEnemy
	{
		GE::GameObject* object = nullptr;
		GE::Math::Vector3 direction;
	};
	LockOnEnemy lockOnEnemy;

	//ヒットストップカウント用
	int hitStopCount;
	//ヒットストップの長さ(フレーム数)
	int hitStopTime;

	//レティクルの位置
	GE::Math::Vector2 center;
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

	static float GameTime;
	//RayCast用
	GE::Math::Vector3 rayPos, rayDir;

	GE::Math::Quaternion quat;
public:
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
	/// <summary>
	/// 操作関係
	/// </summary>
	/// <param name="deltaTime">フレームレートの値</param>
	/// <param name="gameTime">ゲームの時間の速さ</param>
	void Control(float deltaTime);
	/// <summary>
	/// Keyboardで移動操作
	/// </summary>
	void KeyboardMoveControl();
	/// <summary>
	/// 前方にいて最も近い敵を求める
	/// </summary>
	void SearchNearEnemy();
	/// <summary>
	/// ロックオンして攻撃
	/// </summary>
	void LockOn();
	/// <summary>
	/// 加速
	/// </summary>
	/// <param name="dash_speed">速さ</param>
	/// <param name="dash_time">ダッシュの長さ（時間）</param>
	/// <param name="deltaTime">フレームレートの値</param>
	/// <param name="gameTime">ゲームの時間の速さ</param>
	void Dash(float dash_speed, float dash_time, float deltaTime);
	/// <summary>
	/// 
	/// </summary>
	void RayCast(float deltaTime);
	//EaseIn関係がよくわからなかったから一時的に追加
	const float easeIn(const float start, const float end, float time);
};