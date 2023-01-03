#include "FieldObjectDeBugTransform.h"

void FieldObjectDeBugTransform::Update(GE::Transform* transform)
{
	target = transform;
	pivotTransform[0].scale = { 100 };
	pivotTransform[1].scale = { 100 };
	pivotTransform[2].scale = { 100 };
	pivotTransform[0].position = target->position + GE::Math::Vector3(target->scale.x * 3, 0, 0);
	pivotTransform[1].position = target->position + GE::Math::Vector3(0, target->scale.x * 10, 0);
	pivotTransform[2].position = target->position + GE::Math::Vector3(0, 0, target->scale.x * 3);
}

void FieldObjectDeBugTransform::Draw(GE::IGraphicsDeviceDx12* graphicsDevice)
{
	material[0].color = GE::Color::Red();
	material[1].color = GE::Color::Blue();
	material[2].color = GE::Color::Green();

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");
	for (int i = 0; i < 3; i++)
	{
		GE::Math::Matrix4x4 modelMatrix = pivotTransform[i].GetMatrix();

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material[i],sizeof(GE::Material)) });
		graphicsDevice->DrawMesh("Sphere");
	}
}
