#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>


class CrashParticle
{
public:
	struct Object
	{
		GE::Transform transform;
		GE::Math::Vector3 velocity;
		GE::Math::Vector3 randomPos;
		GE::Color color;
	};
	bool isFire;
	//オブジェクトの個数
	int objectsCount;
private:
	std::vector<Object> objects;
	float count;
	static float fireTime;

public:
	Object AddObject(GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color);
	Object AddAnimaiotnObject(GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color);
	void Start();
	void Update(float deltaTime);
	void Draw(GE::IGraphicsDeviceDx12* graphicsDevice);
	void Fire(GE::Math::Vector3 pos,GE::Math::Vector3 hitNormal,GE::Color color);
	void OnGui();
	void UnLoad();
};