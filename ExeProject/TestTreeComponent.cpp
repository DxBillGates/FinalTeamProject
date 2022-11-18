#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >

#include"TestTreeComponent.h"

TestTreeComponent::TestTreeComponent()
{

}

void TestTreeComponent::Awake()
{
	GE::Utility::Printf("TestTreeComponent Awake()\n");
}

void TestTreeComponent::Start()
{
	GE::Utility::Printf("TestTreeComponent Start()\n");

	gameObject->SetColor(GE::Color::Red());
	transform->position = {0,400,-10000};
	transform->scale = { 50,50,50 };

}

void TestTreeComponent::Update(float deltaTime)
{
	
	//‰ñ“]ˆ—
	transform->rotation = {};
}

void TestTreeComponent::Draw()
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	GE::Material material;
	material.color = gameObject->GetColor();
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();


	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("Tree1");
}

void TestTreeComponent::LateDraw()
{

}

void TestTreeComponent::OnCollision(GE::GameObject* other)
{
	GE::Utility::Printf("TestTreeComponent OnCollision(GameObject* other) : hit\n");
}

void TestTreeComponent::OnCollision(GE::ICollider* hitCollider)
{
	GE::Utility::Printf("TestTreeComponent OnCollision(ICollider* other) : hit\n");
}

void TestTreeComponent::OnGui()
{

}