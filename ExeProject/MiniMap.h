#pragma once
#include <GatesEngine/Header/Graphics/IGraphicsDeviceDx12.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h>
#include <GatesEngine/Header/Graphics/CBufferStruct.h>

class MiniMap
{
private:
	GE::GameObjectManager* pGameObjectManager;
	GE::IGraphicsDeviceDx12* pGraphicsDevice;
private:
	void Draw2D(const GE::Math::Vector2& drawPosition,const GE::Math::Vector2& drawSize,const GE::Color& drawColor);
public:
	void Initialize(GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* graphicsDevice);
	void Update(float deltaTime);
	void Draw();
};
