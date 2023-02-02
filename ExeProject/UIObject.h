#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>


class UIObject
{
public:
	struct Object
	{
		bool isDraw = true;
		GE::Transform transform;
		GE::Color color;
		std::string textureName = "texture_null";
		GE::Math::Vector2 texSize = { 1 };
		GE::Math::Vector2 pivotPos = {};
		GE::Math::Vector2 clipSize = { 1 };
	};

	GE::Math::Vector3 cameraPosition;
private:
	std::map<std::string, Object> object;

public:
	static UIObject* GetInstance();
	Object AddObject(GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color, std::string textureName);
	Object AddAnimaiotnObject(GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color, std::string textureName,
		GE::Math::Vector2 texSize, GE::Math::Vector2 clipSize, GE::Math::Vector2 pivotPos = {});
	void SampleSceneStart();
	void DashModeStart();
	void SampleSceneUpdate(float deltaTime);
	void DashModeUpdate(float deltaTime);
	void Draw(GE::IGraphicsDeviceDx12* graphicsDevice);
	void OnGui();
	void UnLoad();
private:
	UIObject() = default;

	~UIObject() = default;
	void operator=(const UIObject& obj) = delete;
	UIObject(const UIObject& obj) = delete;

	Object GetTagObject(std::string tag);
};