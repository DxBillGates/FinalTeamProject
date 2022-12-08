#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 

class CollectTex : public GE::Component
{
public:
	GE::Math::Vector3 position;
	GE::Math::Vector3 scale;
	GE::Math::Quaternion rotation;
	std::string tag;

	float pivotPosX;
	float texSizeX;
	float clipSizeX;

	enum class CollectName
	{
		collect,
		maxCollect,
		symbol,
	};

	int num = 0;

public:

	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;

};

class Collect
{
private:
	int collectCount = 0;//収集物
	int maxCollect = 5;//収集物(最大数)

public:
	std::string tag;
public:
	static Collect* GetInstance();
	Collect();

	void Start(GE::GameObjectManager* gameObjectManager);
	void Update(const int& count, const int& goalCollect);

	const int& GetCollectCount() { return collectCount; }
	const int& GetMaxCollect() { return maxCollect; }

	//テクスチャ生成
	void Create(const std::string& gui_tag, const std::string& tex_tag, GE::GameObjectManager* gameObjectManager, float posX, float scaleX, int collectNum);
};

