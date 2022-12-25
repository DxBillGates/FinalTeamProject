#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics\Window.h>
#include <GatesEngine/Header/GUI\GUIManager.h>

#include"StartTree.h"

int StartTree::goalCollect = 5;
int StartTree::collectCount;
bool StartTree::isCollect;

StartTree::StartTree()
{

}

void StartTree::Awake()
{
	GE::Utility::Printf("TestTreeComponent Awake()\n");
}

void StartTree::Start()
{
	GE::Utility::Printf("TestTreeComponent Start()\n");

	gameObject->SetColor(GE::Color::Red());
	transform->scale = { 1 };
	collectCount = 0;
	isCollect = false;

}

void StartTree::Update(float deltaTime)
{
	if (collectCount < goalCollect)
	{

	}
	else
	{
		isCollect = true;
	}

	transform->rotation = GE::Math::Quaternion::Euler(rotation_euler);
}

void StartTree::Draw()
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	GE::Material material;
	material.color = body_Color;
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("Tree2");

	graphicsDevice->SetShader("DefaultMeshWithTextureShader");
	material.color = leaf_Color;
	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("tree_leaf3")->GetSRVNumber() });

	graphicsDevice->DrawMesh("Tree_Leaf3");

	gameObject->SetColor(body_Color);
}

void StartTree::LateDraw()
{

}

void StartTree::OnCollision(GE::GameObject* other)
{
	//GE::Utility::Printf("TestTreeComponent OnCollision(GameObject* other) : hit\n");
}

void StartTree::OnCollision(GE::ICollider* hitCollider)
{
	//GE::Utility::Printf("TestTreeComponent OnCollision(ICollider* other) : hit\n");
}

void StartTree::OnGui()
{
	ImGui::InputFloat3("Rotation_Euler", rotation_euler.value);
}