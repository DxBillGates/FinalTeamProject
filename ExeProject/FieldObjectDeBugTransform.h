#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>
#include <GatesEngine/Header/GameFramework/Collision/CollisionManager.h>

class FieldObjectDeBugTransform
{
private:
	GE::InputDevice* inputDevice;
	GE::IGraphicsDeviceDx12* graphicsDevice;
public:
	bool isActive;
	enum  class Statas
	{
		DRAG_X,
		DRAG_Y,
		DRAG_Z,
		NONE,
	};
	enum class Trans_State
	{
		POSITION,
		ROTATION,
		SCALE,
	};
	struct Object
	{
		GE::GameObject* target = nullptr;
		std::vector<GE::SphereCollider> coll;
		GE::Transform pivotTransform[3];
		GE::Math::Vector3 pivotLength;
		Statas statas = Statas::NONE;
	};

	Trans_State state = Trans_State::POSITION;
	GE::Material material[3];
	GE::Math::Vector3 rayPos;
	GE::Math::Vector3 rayDir;
	std::vector<Object> objects;
public:
	static FieldObjectDeBugTransform* GetInstance();
	void Update();
	void Draw();
	void UnLoad();

	void SetInputDevice(GE::InputDevice* inputDevice);
	void SetGraphicsDevice(GE::IGraphicsDeviceDx12* gDevice);

	void AddTarget(GE::GameObject* gameobject, GE::Math::Vector3 pivotLength = { 3.0f });
private:
	FieldObjectDeBugTransform() = default;

	~FieldObjectDeBugTransform() = default;
	void operator=(const FieldObjectDeBugTransform& obj) = delete;
	FieldObjectDeBugTransform(const FieldObjectDeBugTransform& obj) = delete;
};

