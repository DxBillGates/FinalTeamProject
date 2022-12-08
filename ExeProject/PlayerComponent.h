#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/Graphics/SkinMeshAnimator.h>
#include <GatesEngine/Header/Audio/AudioManager.h>

class PlayerComponent : public GE::Component
{
public:
	static float current_speed;		//現在のスピード
	static float normal_speed;		//通常時のスピード
	static float frameRate;
private:
	static GE::Math::Vector3 onTheTreePosition;	//木の上で体の高さ調整用
	static GE::Math::Vector3 gravity;	//重力
	static float rayHitSecond;		//照準を合わせる長さ（秒数）
	static int hitStopTime;
	static float damageSpeed;
	static float pushStartTime;		//キーを押してから操作できるようになるまでのカウント
	static float stayLandLerpTime;	//木に戻るラープ
	static int collectMax;			//収集物の同時にもてる最大個数
	static float body_direction_LerpTime; //秒数
	static float worldRadius;				//世界の大きさの半径(端っこの壁までの距離)
	static float lockOnLength;				//敵をロックオンできる距離

	static GE::AudioManager* audioManager;
	GE::InputDevice* inputDevice;
	GE::Math::Vector3 gyro;
	GE::Math::Vector3 accelerometer;
	GE::SkinMeshAnimator animator;

	bool isLockOnStart;				//ロックオン処理を呼ぶフラグ
	bool isLockOn;					//ロックオンして発射待機中フラグ

	float dashEasingCount;			//スピード遷移のカウント
	float stayLandLerpEasingCount;	//着陸する遷移カウント
	float rayHitCount;				//何フレーム照準をあわせているか
	int collectCount;				//取集物を何個集めたか
	int hitStopCount;				//ヒットストップカウント用
	float startCouunt;				//開始時のカウント
	int body_direction_LerpCount;	//元の姿勢に戻るときの遷移

	int statasChangeCount;			//インプットが次のPlayerStatasでも動かないように1フレーム繰越すよう
	struct LockOnEnemy
	{
		GE::GameObject* object = nullptr;
		GE::Math::Vector3 direction;
	};
	LockOnEnemy lockOnEnemy;

	//レティクルの位置
	GE::Math::Vector2 center;

	GE::Math::Quaternion quat;
	GE::Math::Vector3 body_direction;				//体の向き計算用
	GE::Math::Quaternion body_direction_LockOn;		//ロックオン時の体の向き計算用
	bool is_rayCast_active;							//レイキャストの照準が使われてるか

	GE::Math::Vector3 currentPosition;//巣に着陸するときのラープ用
public:
	enum class PlayerStatas
	{
		STOP_DEBUG,		//その場に停止
		MOVE,			//通常移動
		DASH,			//ダッシュ
		CRASH,			//壁に激突して墜落
		LOCKON_SHOOT,	//敵をロックオンして攻撃
		GO_TREE,		//巣の木に着陸中
		STAY_TREE,		//木で待機
	};
	PlayerStatas statas;				//Playerの状態
	//RayCast用
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
	/// 一定以上のスピードになっているかを取得するため
	/// </summary>
	/// <returns></returns>
	static bool IsSpeedy();
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
	/// <param name="direction">向き</param>
	/// <param name="loop">Trueの間はダッシュし続ける。Falseになるとdash_timeのフレーム数かけて減速する。最初からfalseだとdash_timeフレーム数加速する</param>
	void Dash(float dash_speed, float dash_time, float deltaTime, GE::Math::Vector3 direction, bool loop = false);
	/// <summary>
	/// 
	/// </summary>
	void RayCast(float deltaTime);

	void LookDirection(GE::Math::Vector3 direction);

	void Reflection();
	//EaseIn関係がよくわからなかったから一時的に追加
	const float easeIn(const float start, const float end, float time);
public:
	GE::Math::Vector3 GetDirection();
	static void SetAudioManager(GE::AudioManager* a);
};