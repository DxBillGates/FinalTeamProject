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

void StartTree::DrawShadow()
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

	TreeLeafDraw();
}

void StartTree::TreeLeafDraw()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	GE::Material material;
	material.color = leaf_Color;
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	graphicsDevice->SetShader("DefaultMeshWithTextureAndAdsCompositiongShader");
	material.color = leaf_Color;
	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetShaderResource({ 16,graphicsDevice->GetTextureManager()->Get("texture_tree_leaf")->GetSRVNumber() });
	// �A�j���[�V�����̏��
	GE::TextureAnimationInfo textureAnimationInfo;
	// �摜�̌��T�C�Y
	GE::Math::Vector2 textureSize = { 1,1 };
	textureAnimationInfo.textureSize = textureSize;
	// ���摜�̃T�C�Y����ǂ�����Đ؂蔲�����@��) ���T�C�Y��100*100�Ŕ��������\���������Ȃ�{50,100}�ɂ���
	// textureSize�ƈꏏ�ɂ���Ɛ؂蔲���ꂸ�ɕ`�悳���
	textureAnimationInfo.clipSize = { 1,1 };
	// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
	textureAnimationInfo.pivot = { 0,0 };
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });

	graphicsDevice->DrawMesh("Tree_Leaf3");
}

void StartTree::Draw()
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshWithShadowShader");

	renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });

	GE::Material material;
	material.color = body_Color;
	gameObject->SetColor(body_Color);
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("Tree2");

	//TreeLeafDraw();
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