#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 

class TimeTex : public GE::Component
{
public:
	GE::Math::Vector3 position;
	GE::Math::Vector3 scale;
	GE::Math::Quaternion rotation;
	std::string tag;

	float pivotPosX;
	float texSizeX;

	enum class TimeName
	{
		minutes,
		tenSeconds,
		oneSeconds,
	};

	int num = 0;

public:

	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;

};

class TimeLimit 
{
private:
	float time;
	int minutes = 0;//分数の計算変数
	int tenSeconds = 0;//秒数の計算変数(十の位)
	int oneSeconds = 0;//秒数の計算変数(一の位)
	bool timeOver = false;//タイムオーバーフラグ

public:
	std::string tag;
public:
	static TimeLimit* GetInstance();
	void Start(GE::GameObjectManager* gameObjectManager);
	void Update();
	void AddSeconds(const int& seconds);//時間追加したいときに呼ぶ関数

	const int& GetMinutes() { return minutes; }//分数のゲット関数
	const int& GetTenSeconds() { return tenSeconds; }//秒数のゲット関数(十の位)
	const int& GetOneSeconds() { return oneSeconds; }//秒数のゲット関数(一の位)
	const bool& GetTimeOver() { return timeOver; }//タイムオーバーフラグのゲット関数

	//テクスチャ生成
	void Create(const std::string& gui_tag, const std::string& tex_tag, GE::GameObjectManager* gameObjectManager, float posX, float scaleX, int timeNum);
private:
	TimeLimit() = default;
	~TimeLimit() = default;

	// 代入、コピーの禁止
	void operator=(const TimeLimit& obj) = delete;
	TimeLimit(const TimeLimit& obj) = delete;
};

