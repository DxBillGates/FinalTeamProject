#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

#include"FieldObject.h"


FieldObjectComponent::FieldObjectComponent()
{

}

void FieldObjectComponent::Awake()
{
	GE::Utility::Printf("FieldObject Awake()\n");
}

void FieldObjectComponent::Start()
{
	GE::Utility::Printf("FieldObject Start()\n");
}
void FieldObjectComponent::Update(float deltaTime)
{

}

void FieldObjectComponent::DrawShadow()
{
	if (!isDraw)return;

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

void FieldObjectComponent::Draw()
{
	if (!isDraw)return;
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
	if (shaderName != "DefaultMeshShader")
	{
		renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
		renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get(textureName)->GetSRVNumber() });
	}
	if (shaderName == "DefaultMeshWithShadowShader")
	{
		GE::CameraInfo cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
		cameraInfo.lightMatrix = gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->GetVPMatrix();

		renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
		renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });
	}
	if (shaderName == "CelestialSphereShader")
	{
		renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
		GE::Math::Vector2 sunsUV = gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->GetUV();
		renderQueue->AddSetShaderResource({ 16,graphicsDevice->GetTextureManager()->Get(textureName)->GetSRVNumber() });
		renderQueue->AddSetConstantBufferInfo({ 5,cbufferAllocater->BindAndAttachData(5, &sunsUV,sizeof(GE::Math::Vector2)) });
	}
	if (shaderName == "DefaultMeshWithTextureAndAdsCompositiongShader")
	{
		GE::CameraInfo cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
		cameraInfo.lightMatrix = gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->GetVPMatrix();

		renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
		renderQueue->AddSetShaderResource({ 16,graphicsDevice->GetTextureManager()->Get(textureName)->GetSRVNumber() });
		renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });
	}
	graphicsDevice->DrawMesh(modelName);
}

void FieldObjectComponent::LateDraw()
{

}

void FieldObjectComponent::OnCollision(GE::GameObject* other)
{
	GE::Utility::Printf("FieldObject OnCollision(GameObject* other) : hit\n");
}

void FieldObjectComponent::OnCollision(GE::ICollider* hitCollider)
{
	GE::Utility::Printf("FieldObject OnCollision(ICollider* other) : hit\n");
}

void FieldObjectComponent::OnGui()
{
	ImGui::Checkbox("IsDraw", &isDraw);
}

//FieldObject* FieldObject::GetInstance()
//{
//	static FieldObject instance;
//	return &instance;
//}
//
//void FieldObject::Update()
//{
//
//}
//
//void FieldObject::SetGraphicsDevice(GE::IGraphicsDeviceDx12* gDevice)
//{
//	graphicsDevice = gDevice;
//}
//
//void FieldObject::AddTarget(GE::GameObject* gameobject)
//{
//	Object result;
//	result.transform = gameobject;
//	objects.push_back(result);
//}
//
//
//void FieldObject::Draw()
//{
//	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
//	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();
//
//	for (auto& obj : objects)
//	{
//		graphicsDevice->SetShader("DefaultMeshShader");
//		GE::Math::Matrix4x4 modelMatrix = obj.transform.GetMatrix();
//
//		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
//		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material[i],sizeof(GE::Material)) });
//		graphicsDevice->DrawMesh("Sphere");
//	}
//}
//
//void FieldObject::UnLoad()
//{
//	objects.clear();
//}