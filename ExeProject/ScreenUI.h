#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>


class ScreenUIManager
{
public:
	struct SpriteInfo
	{
		bool isDraw = false;
		GE::Transform transform;
		GE::Color color;
		std::string textureName = "texture_null";
		GE::Math::Vector2 texSize = { 1 };
		GE::Math::Vector2 pivotPos = {};
		GE::Math::Vector2 clipSize = { 1 };

		float lerpCount = 0.0f;
	};
	GE::Math::Vector2 viveVelocity;
private:
	struct  LoadObj
	{
		GE::Math::Vector3 pos;
		GE::Math::Vector3 rot;
		GE::Math::Vector3 scale;
		GE::Color col;
	};
	std::map<std::string, SpriteInfo> object;

	float a = 0.f;

	GE::Math::Vector2 winSize;
	GE::Math::Vector2 center;

public:
	static ScreenUIManager* GetInstance();
	void NormalModeStart();
	void DashModeStart();
	void NormalModeUpdate(float deltaTime);
	void DashModeUpdate(float deltaTime);
	void DrawSprite(GE::IGraphicsDeviceDx12* graphicsDevice);
	void TimeLimitActive(float deltaTime);
	GE::Math::Vector3 Vivlate(float deltaTime);
	//ファイルに保存されている座標取得
	void LoadPosition(const std::string& filename);
	//現在の座標をファイルに保存
	void SaveCurrentPosition(const std::string& filename);

	void UnLoad();
private:
	ScreenUIManager() = default;

	~ScreenUIManager() = default;
	void operator=(const ScreenUIManager& obj) = delete;
	ScreenUIManager(const ScreenUIManager& obj) = delete;

	SpriteInfo Set(GE::Math::Vector3 pos, GE::Math::Vector3 scale, GE::Color color, std::string textureName);
	SpriteInfo Set(GE::Math::Vector3 pos, GE::Math::Vector3 scale, GE::Color color, std::string textureName, GE::Math::Vector2 texSize, GE::Math::Vector2 clipSize);

	float SetLerp(std::string name, float lerpTime, float deltaTime);
	//タイトルメニューを描画するか
	void TitleMenuActive(bool isActive);
	//オプションメニューを描画するか
	void OptionMenuActive(bool isActive);

};
