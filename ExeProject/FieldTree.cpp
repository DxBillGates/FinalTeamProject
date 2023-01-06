#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

#include "FieldTree.h"
#include "FieldObjectDeBugTransform.h"


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
}
void FieldTree::Update(float deltaTime)
{
}

void FieldTree::DrawShadow()
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

	TreeLeafDraw();
}
void FieldTree::TreeLeafDraw()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshWithTextureAndAdsCompositiongShader");

	GE::Material material;
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	material.color = GE::Color(0.5f, 0.5f, 0.5f, 1.0f);
	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetShaderResource({ 16,graphicsDevice->GetTextureManager()->Get("texture_tree_leaf")->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });
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

void FieldTree::Draw()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshWithShadowShader");


	GE::Material material;
	material.color = body_Color;
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });

	GE::CameraInfo cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
	cameraInfo.lightMatrix = gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->GetVPMatrix();
	gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->SetDirectionalLightInfo();
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });

	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("tree1");

	//material.color = leaf_Color;
	//renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	//renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	//graphicsDevice->DrawMesh("tree_leaf1");
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
}

void FieldTree::UnLoad()
{
}
