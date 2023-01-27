#pragma once
#include<GatesEngine/Header/Graphics/Camera3DDebug.h>
#include <GatesEngine/Header/GameFramework/Component/Component.h>

//カメラ制御を担当するクラス
class CameraControl
{
private:
	GE::IGraphicsDeviceDx12* graphicsDevice = nullptr;

	GE::Math::Vector3 position;		//カメラ位置
	GE::Math::Vector3 target;		//カメラ注視座標
	GE::GameObject* targetObject;

	float dir;

	float current_cameraDistance;//カメラとPlayerの距離
	float normal_cameraDistance;//通常時カメラとPlayerの距離
	float dash_cameraDistance;	//ダッシュ時カメラとPlayerの距離

	//カメラシェイク用
	int shakeFlame;
	GE::Math::Vector2 range;
	GE::Math::Vector3 cameraShake;
	//カメラのブレ用
	float count;

public:
	static CameraControl* GetInstance();

	void Create();
	void Initialize();	//初期化
	void Update(float deltaTime);		//更新

	void Distance_current(float value) { current_cameraDistance = value; }	//カメラとの距離
	void Distance_normal(float value) { normal_cameraDistance = value; }	//普段の距離
	void Distance_dash(float value) { dash_cameraDistance = value; }		//ダッシュ時の距離
	void SetGraphicsDevice(GE::IGraphicsDeviceDx12* graphicsDevice);		//取得

	/// <summary>
	/// カメラシェイク開始時設定
	/// </summary>
	/// <param name="range">揺らす範囲</param>
	/// <param name="flame">揺らすフレーム数</param>
	void ShakeStart(GE::Math::Vector2 range = { 10,10 }, int flame = 5);
	void SetTargetObject(GE::GameObject* t) { targetObject = t; }
	void SetClearCameraPosition();
	void SetOverCameraPosition();

private:
	CameraControl() = default;
	~CameraControl() = default;

	// 代入、コピーの禁止
	void operator=(const CameraControl & obj) = delete;
	CameraControl(const CameraControl & obj) = delete;

	void Shake(float deltaTime);
	//EaseIn関係がよくわからなかったから一時的に追加
	const float easeIn(const float start, const float end, float time);
};

