#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class FieldObjectDeBugTransform
{
private:
	GE::Transform*target;

	GE::Transform pivotTransform[3];
	GE::Material material[3];
public:
	void Start();
	void Update(GE::Transform*transform);
	void Draw(GE::IGraphicsDeviceDx12* graphicsDevice);
	void UnLoad();
};

