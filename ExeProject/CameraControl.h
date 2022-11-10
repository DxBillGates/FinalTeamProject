#pragma once
#include<GatesEngine/Header/Graphics/Camera3DDebug.h>
#include <GatesEngine/Header/GameFramework/Component/Component.h>

//カメラ制御を担当するクラス
class CameraControl
{
private:
	GE::IGraphicsDeviceDx12* graphicsDevice = nullptr;

	GE::Math::Vector3 target;	//方向
	GE::Math::Vector3 direction;	//方向
	GE::Math::Vector3 position;		//カメラ位置
	GE::Math::Vector3 other;		//相手
	GE::Math::Axis otherAxis;		//相手のAxis

	float dir;					//追従するカメラのPlayerから見た角度
	float current_cameraDistance;//カメラとPlayerの距離
	float normal_cameraDistance;//通常時カメラとPlayerの距離
	float dash_cameraDistance;	//ダッシュ時カメラとPlayerの距離

	//カメラシェイク用
	int shakeFlame;
	float count;
	GE::Math::Vector2 range;
	GE::Math::Vector3 cameraShake;

public:
	static CameraControl* GetInstance();
	CameraControl() = default;
	~CameraControl() = default;

	// 代入、コピーの禁止
	void operator=(const CameraControl& obj) = delete;
	CameraControl(const CameraControl& obj) = delete;

	void Create();
	void Initialize();	//初期化
	void Update();		//更新

	void Distance_current(float value) { current_cameraDistance = value; }	//カメラとの距離
	void Distance_normal(float value) { normal_cameraDistance = value; }	//普段の距離
	void Distance_dash(float value) { dash_cameraDistance = value; }		//ダッシュ時の距離
	void SetDir(float value) { dir = value; }								//向き
	void SetGraphicsDevice(GE::IGraphicsDeviceDx12* graphicsDevice);		//取得
	void SetOtherPos(GE::Math::Vector3& pos);								//カメラを向ける相手
	void SetOtherAxis(GE::Math::Axis axis);

	void Direction(GE::Math::Vector3& target);								//方向計算
	void DashCam(float dashEasingCount, float dash_time = 100);				//ダッシュ中計算

	/// <summary>
	/// カメラシェイク開始時設定
	/// </summary>
	/// <param name="range">揺らす範囲</param>
	/// <param name="flame">揺らすフレーム数</param>
	void ShakeStart(GE::Math::Vector2 range = { 10,10 }, int flame = 5);

	void CameraVivlate();
private:
	void Shake();
	//EaseIn関係がよくわからなかったから一時的に追加
	const float easeIn(const float start, const float end, float time);
};

