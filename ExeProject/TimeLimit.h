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

	float pivotPos;
	float texSize;

	enum TimeName
	{
		minutes,
		tenSeconds,
		oneSeconds,
	};

	TimeName tName;
	int num = 0;

public:

	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;

};

class TimeLimit 
{
private:
	int timer = 3;//制限時間(分指定)
	int minutes = 0;//分数の描画用変数
	int tenSeconds = 0;//秒数の十の位の描画用変数
	int oneSeconds = 0;//秒数の一の位の描画用変数
	const int frameRate = 144;//フレームレート
	bool timeOver = false;

public:
	std::string tag;
public:
	static TimeLimit* GetInstance();
	TimeLimit();

	void Start(GE::GameObjectManager* gameObjectManager);
	void Update();

	const int& GetMinutes() { return minutes; }
	const int& GetTenSeconds() { return tenSeconds; }
	const int& GetOneSeconds() { return oneSeconds; }
	const bool& GetTimeOver() { return timeOver; }

	//テクスチャ生成
	void Create(const std::string& gui_tag, const std::string& tex_tag, GE::GameObjectManager* gameObjectManager, float posX, float scaleX, int animeNum);
};

