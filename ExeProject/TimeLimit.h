#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include<GatesEngine/Header/Audio/AudioManager.h>

class TimeLimit 
{
private:
	float time;//制限時間
	int minutes = 0;//分数の計算変数
	int tenSeconds = 0;//秒数の計算変数(十の位)
	int oneSeconds = 0;//秒数の計算変数(一の位)
	bool limit = false;//タイムオーバー前の演出
	bool timeOver = false;//タイムオーバーフラグ

	float angle = 0.0f;//大きくなる早さ
	float addAngle = 0.075f;//早さの数値
	float range = 0.5f;//大きくなる範囲
	GE::Math::Vector3 tScale = {};//描画サイズの変更変数
	int interval = 0;//SEの鳴る間隔変数

public:
	std::string tag;
	//開始しても初めて飛び立つまでカウントしない用
	bool isActive;
public:
	static TimeLimit* GetInstance();
	void Start(GE::GameObjectManager* gameObjectManager);
	void Update(GE::AudioManager* audioManager);
	void AddSeconds(const int& seconds);//時間追加したいときに呼ぶ関数

	const int& GetMinutes() { return minutes; }//分数のゲット関数
	const int& GetTenSeconds() { return tenSeconds; }//秒数のゲット関数(十の位)
	const int& GetOneSeconds() { return oneSeconds; }//秒数のゲット関数(一の位)
	const bool& GetLimit() { return limit; }//タイムオーバー前の演出のゲット関数
	const bool& GetTimeOver() { return timeOver; }//タイムオーバーフラグのゲット関数
	const GE::Math::Vector3& GetTScale() { return tScale; }//描画サイズのゲット関数

	//テクスチャ生成
	void Create(const std::string& gui_tag, const std::string& tex_tag, GE::GameObjectManager* gameObjectManager, float posX, float scaleX, int timeNum);
private:
	TimeLimit() = default;
	~TimeLimit() = default;

	// 代入、コピーの禁止
	void operator=(const TimeLimit& obj) = delete;
	TimeLimit(const TimeLimit& obj) = delete;
};

