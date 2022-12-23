#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h>

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

void FieldObject::DrawShadow()
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	GE::Material material;
	material.color = gameObject->GetColor();
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

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

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	if (shaderName == "DefaultMeshWithTextureShader")
	{
		renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
		renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get(textureName)->GetSRVNumber() });
	}
	graphicsDevice->DrawMesh(modelName);
}

void FieldObject::Draw()
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader(shaderName);

	GE::Material material;
	material.color = gameObject->GetColor();
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

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

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	if (shaderName == "DefaultMeshWithTextureShader")
	{
		renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
		renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get(textureName)->GetSRVNumber() });
	}
	if (shaderName == "DefaultMeshWithShadowShader")
	{
		GE::GameObjectManager* gameObjectManager = gameObject->GetGameObjectManager();
		GE::GameObject* directionLight = gameObjectManager->FindGameObjectWithTag("directionLight", "directionLight");
		GE::Transform* directionLightPos = directionLight->GetTransform();
		GE::CameraInfo cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
		GE::Math::Matrix4x4 lightMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo(directionLightPos->position, { 0,-1,0 }, { 0,0,1 });
		GE::Math::Matrix4x4 projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Math::Vector2(20000), 1, 20000);
		cameraInfo.lightMatrix = lightMatrix * projMatrix;

		renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
		renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber()});
	}
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