#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >

#include"FieldObject.h"


FieldObject::FieldObject()
{

}

void FieldObject::Awake()
{
	GE::Utility::Printf("FieldObject Awake()\n");
}

void FieldObject::Start()
{
	GE::Utility::Printf("FieldObject Start()\n");
}
void FieldObject::Update(float deltaTime)
{

}

void FieldObject::Draw()
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader(shaderName);

	GE::Material material;
	material.color = gameObject->GetColor();
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh(modelName);
}

void FieldObject::LateDraw()
{

}

void FieldObject::OnCollision(GE::GameObject* other)
{
	GE::Utility::Printf("FieldObject OnCollision(GameObject* other) : hit\n");
}

void FieldObject::OnCollision(GE::ICollider* hitCollider)
{
	GE::Utility::Printf("FieldObject OnCollision(ICollider* other) : hit\n");
}

void FieldObject::OnGui()
{

}