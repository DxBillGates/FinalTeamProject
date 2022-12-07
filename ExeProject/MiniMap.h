#pragma once
#include <GatesEngine/Header/Graphics/IGraphicsDeviceDx12.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h>
#include <GatesEngine/Header/Graphics/CBufferStruct.h>

class MiniMap
{
private:
	GE::GameObjectManager* pGameObjectManager;
	GE::IGraphicsDeviceDx12* pGraphicsDevice;
	float angle;

	struct Draw2DInfo
	{
		GE::Math::Vector2 pos;
		GE::Math::Vector2 size;
		GE::Color color;
	};
private:
	void Draw2D(const Draw2DInfo& drawInfo);
	void Draw2D(const Draw2DInfo& drawInfo,const GE::Math::Vector2& mapOffset,const GE::Math::Vector3& minPos,const GE::Math::Vector3& maxPos);
public:
	void Initialize(GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* graphicsDevice);
	void Update(float deltaTime,float direction = 0);
	void Draw();
};
