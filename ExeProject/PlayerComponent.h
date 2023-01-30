#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/Graphics/SkinMeshAnimator.h>
#include <GatesEngine/Header/Audio/AudioManager.h>
#include"CrashParticle.h"

class PlayerComponent : public GE::Component
{
public:
	static float current_speed;		//現在のスピード
	static float normal_speed;		//通常時のスピード
	static float frameRate;
	static bool isGoTree;
	static bool dashMode;
	static int combo;				//何コンボ？
private:
	static GE::Math::Vector3 onTheTreePosition;	//木の上で体の高さ調整用
	static GE::Math::Vector3 gravity;	//重力
	static int hitStopTime;
	static float pushStartTime;		//キーを押してから操作できるようになるまでのカウント
	static float stayLandLerpTime;	//木に戻るラープ
	static int colectMax;			//収集物の同時にもてる最大個数
	static float worldRadius;				//世界の大きさの半径(端っこの壁までの距離)
	static float lockOnLength;				//敵をロックオンできる距離
	static float moreTimesLockOnLength;		//連続で二回目以降の敵をロックオンできる距離
	static int lockOnInterval;				//ロックオンのインターバルの長さ
	static float comboInterval;				//連続で敵を倒してコンボとなるまでのインターバル(大きいほどコンボにつながりやすい)

	GE::AudioManager* audioManager;
	GE::InputDevice* inputDevice;
	GE::Math::Vector3 gyro;
	GE::Math::Vector3 accelerometer;
	GE::SkinMeshAnimator animator;

	bool isDraw;					//体を描画するか//デバッグ中に体邪魔だったら消せ

	bool isLockOnStart;				//ロックオン処理を呼ぶフラグ
	bool isLockOn;					//ロックオンして発射待機中フラグ

	float dashEasingCount;			//スピード遷移のカウント
	float stayLandLerpEasingCount;	//着陸する遷移カウント
	int colectCount;				//取集物を何個集めたか
	float hitStopCount;				//ヒットストップカウント用
	float startCouunt;				//開始時のカウント
	int lockOnIntervalCount;		//ロックオンのインターバルのカウント
	float comboCount;

	int statasChangeCount;			//インプットが次のPlayerStatasでも動かないように1フレーム繰越すよう
	struct LockOnEnemy
	{
		GE::GameObject* object = nullptr;
		GE::Math::Vector3 direction;
	};
	LockOnEnemy lockOnEnemy;
	CrashParticle crashParticle;

	//レティクルの位置
	GE::Math::Vector2 center;

	//ロックオン時の向き
	GE::Math::Vector3 lockOnDashDirection;

	GE::Math::Vector3 totalGyro;
	GE::Math::Quaternion preQuat;
	GE::Math::Quaternion quat;
	GE::Math::Vector3 body_direction;				//体の向き計算用

	GE::Math::Vector3 currentPosition;//巣に着陸するときのラープ用
public:
	enum class PlayerStatas
	{
		TITLE,			//タイトル、木で待機
		TITLE_MENU,		//タイトルメニュー、木で待機
		DEBUG,			//その場に停止
		MOVE,			//通常移動
		DASH,			//ダッシュ
		CRASH,			//壁に激突して墜落
		LOCKON_SHOOT,	//敵をロックオンして攻撃
		GO_TREE,		//巣の木に着陸中
		STAY_TREE,		//木で待機
		OVER,			//
	};
	static PlayerStatas statas;				//Playerの状態
	//RayCast用
	GE::Math::Vector3 rayPos, rayDir;

public:
	PlayerComponent();
	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void DrawShadow() override;
	void Draw() override;
	void LateDraw() override;
	void OnCollision(GE::GameObject* other) override;
	void OnCollisionEnter(GE::GameObject* other)override;
	void OnCollisionExit(GE::GameObject* other)override;
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
	void KeyboardMoveControl(float deltaTime);

	/// <summary>
	/// 前方にいて最も近い敵を求める
	/// </summary>
	void SearchNearEnemy(bool isForward = false);
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

	void NormalDash(float dash_speed, float dash_time, float deltaTime);

	void Reflection(GE::Math::Vector3 normal, bool reflection = false);
	//EaseIn関係がよくわからなかったから一時的に追加
	const float easeIn(const float start, const float end, float time);

public:
	GE::Math::Vector3 GetDirection();
	void SetAudioManager(GE::AudioManager* a);
};