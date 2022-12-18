#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>


class UIObject
{
public:
	struct Object
	{
		std::string tag = "Tag_Ga_Naiyo";
		GE::Transform transform;
		GE::Color color;
		std::string textureName = "texture_null";
	};

	GE::Math::Vector3 cameraPosition;
private:
	std::vector<Object> normalUI;
	std::vector<Object> AnimationUI;

	float pivotPosX;
	float texSizeX;
	float clipSizeX;

	int num = 0;


public:
	static UIObject*GetInstance();
	Object AddObject(std::string tag, GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color, std::string textureName);
	void Start();
	void Update(float deltaTime);
	void Draw(GE::IGraphicsDeviceDx12* graphicsDevice);
	void OnGui();
	void UnLoad();
private:
	UIObject() = default;

	~UIObject() = default;
	void operator=(const UIObject & obj) = delete;
	UIObject(const UIObject & obj) = delete;

	GE::Math::Quaternion LookDirection(GE::Math::Vector3 direction);
};