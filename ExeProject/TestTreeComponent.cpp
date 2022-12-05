#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics\Window.h>
#include <GatesEngine/Header/GUI\GUIManager.h>

#include"TestTreeComponent.h"

GE::Math::Vector3 TestTreeComponent::position = { 0,1400,-10000 };
int TestTreeComponent::goalCollect = 10;
int TestTreeComponent::collectCount;
bool TestTreeComponent::isCollect;

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
	transform->position = position;
	transform->scale = { 200,200,200 };
	collectCount = 0;
	isCollect = false;

}

void TestTreeComponent::Update(float deltaTime)
{
	if (collectCount < goalCollect)
	{


	}
	else
	{
		isCollect = true;
	}
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
	//GE::Utility::Printf("TestTreeComponent OnCollision(GameObject* other) : hit\n");
}

void TestTreeComponent::OnCollision(GE::ICollider* hitCollider)
{
	//GE::Utility::Printf("TestTreeComponent OnCollision(ICollider* other) : hit\n");
}

void TestTreeComponent::OnGui()
{

}