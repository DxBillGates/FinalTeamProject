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