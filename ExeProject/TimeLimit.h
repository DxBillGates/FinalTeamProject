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
	
public:

	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;

};

class TimeLimit 
{
private:
	int timer = 1;//制限時間
	int minutes = 0;//分数
	int tenSeconds = 0;//秒数の十の位
	int oneSeconds = 0;//秒数の一の位
	int fps = 144;//フレームレート


public:
	std::string tag;
public:
	TimeLimit(const int& timer);

	void Start(GE::GameObjectManager* gameObjectManager);
	void Update();

	//テクスチャ生成
	void Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager,float shift);
};

