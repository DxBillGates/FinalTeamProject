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

void FieldObject::Draw()
{

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