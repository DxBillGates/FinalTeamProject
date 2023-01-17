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
	};
private:
	struct  LoadObj
	{
		GE::Math::Vector3 pos;
		GE::Math::Vector3 rot;
		GE::Math::Vector3 scale;
		GE::Color col;
	};
	std::map<std::string,SpriteInfo> object;

	int a = 0;
public:
	static ScreenUIManager* GetInstance();
	void Start();
	void Update(float deltaTime);
	void DrawSprite(GE::IGraphicsDeviceDx12* graphicsDevice);
	//�t�@�C���ɕۑ�����Ă�����W�擾
	void LoadPosition(const std::string& filename);
	//���݂̍��W���t�@�C���ɕۑ�
	void SaveCurrentPosition(const std::string& filename);

	void UnLoad();
private:
	ScreenUIManager() = default;

	~ScreenUIManager() = default;
	void operator=(const ScreenUIManager& obj) = delete;
	ScreenUIManager(const ScreenUIManager& obj) = delete;

	SpriteInfo Set(GE::Math::Vector3 pos, GE::Math::Vector3 scale, GE::Color color, std::string textureName);
};
