#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >

#include "NormalTree.h"

FieldTree::FieldTree()
{

}

void FieldTree::Awake()
{
	GE::Utility::Printf("NormalTree Awake()\n");
}

void FieldTree::Start()
{
	GE::Utility::Printf("NormalTree Start()\n");
	rotation_euler = {};
	scale = 100;
}
void FieldTree::Update(float deltaTime)
{
	transform->rotation = GE::Math::Quaternion::Euler(rotation_euler);
	transform->scale = { scale };
}

void FieldTree::Draw()
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	GE::Material material;
	material.color = body_Color;
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("tree1");
}
void FieldTree::TreeLeafDraw()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshWithTextureShader");

	GE::Material material;
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	material.color = GE::Color(0.5f, 0.5f, 0.5f, 1.0f);
	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("texture_tree_leaf")->GetSRVNumber() });
	// アニメーションの情報
	GE::TextureAnimationInfo textureAnimationInfo;
	// 画像の元サイズ
	GE::Math::Vector2 textureSize = { 1,1 };
	textureAnimationInfo.textureSize = textureSize;
	// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
	// textureSizeと一緒にすると切り抜かれずに描画される
	textureAnimationInfo.clipSize = { 1,1 };
	// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
	textureAnimationInfo.pivot = { 0,0 };
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });


	graphicsDevice->DrawMesh("Tree_Leaf2");


}
void FieldTree::LateDraw()
{

}

void FieldTree::OnCollision(GE::GameObject* other)
{
	GE::Utility::Printf("NormalTree OnCollision(GameObject* other) : hit\n");
}

void FieldTree::OnCollision(GE::ICollider* hitCollider)
{
	GE::Utility::Printf("NormalTree OnCollision(ICollider* other) : hit\n");
}

void FieldTree::OnGui()
{
	ImGui::InputFloat3("Rotation_Euler", rotation_euler.value);
	ImGui::InputFloat("Scale", &scale);
}