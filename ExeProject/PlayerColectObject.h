#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class PlayerColectObject
{
public:
	struct ColectingObject
	{
		GE::Transform transform;
		GE::Color color = { 1,1,1,1 };
		std::string modelName = "Sphere";
		//足に掴んでいるオブジェクトのばらつき
		GE::Math::Vector3 LocalPosition;
		GE::Math::Quaternion LocalRotation;
	};
private:
	std::vector<ColectingObject> colectingObjs;
	ColectingObject fallenObject;
	//player
	GE::GameObject* targetObject;

	GE::Math::Vector3 velocity;
public:
	static PlayerColectObject* GetInstance();
	void Start(int colectMax, GE::GameObject* targetObject);
	void Update(float deltaTime);
	void DrawShadow(GE::IGraphicsDeviceDx12* graphicsDevice);
	void Draw(GE::IGraphicsDeviceDx12* graphicsDevice, GE::GameObjectManager* manager);
	void UnLoad();
	void SetFallen(GE::Math::Vector3 normal);
	void AddObject(std::string modelName);
	void ClearObject();
private:
	PlayerColectObject() = default;

	~PlayerColectObject() = default;
	void operator=(const PlayerColectObject& obj) = delete;
	PlayerColectObject(const PlayerColectObject& obj) = delete;

	ColectingObject SetColectObject(GE::Math::Vector3 pos, GE::Math::Quaternion ro, GE::Math::Vector3 scl,GE::Color color,std::string modelName);
};

