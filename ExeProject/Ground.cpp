#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >

#include"Ground.h"


Ground::Ground()
{

}

void Ground::Awake()
{
	GE::Utility::Printf("Ground Awake()\n");
}

void Ground::Start()
{
	GE::Utility::Printf("Ground Start()\n");

	gameObject->SetColor(GE::Color(0.5f, 0.9f, 0.5f, 1.0f));
	position = { 1000,0,-15000 };
	transform->position = position;
	transform->scale = { 25000,25000,25000 };
	transform->rotation = GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), -5.0f);

}
void Ground::Update(float deltaTime)
{

}

void Ground::Draw()
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	GE::Material material;
	material.color = gameObject->GetColor();
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();


	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("GroundTest");
}

void Ground::LateDraw()
{

}

void Ground::OnCollision(GE::GameObject* other)
{
	GE::Utility::Printf("Ground OnCollision(GameObject* other) : hit\n");
}

void Ground::OnCollision(GE::ICollider* hitCollider)
{
	GE::Utility::Printf("Ground OnCollision(ICollider* other) : hit\n");
}

void Ground::OnGui()
{

}