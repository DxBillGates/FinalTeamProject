#include "ClearBird.h"
#include "FieldObjectManager.h"
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>
#include <GatesEngine/Header\GameFramework/GameSetting.h>

void ClearBird::Awake()
{
}

void ClearBird::Start()
{
	//transform->position = FieldObjectManager::GetInstance()->StartPosition + GE::Math::Vector3(0, 250, 200);
	transform->scale = { 10,10,10 };

	animator = GE::SkinMeshManager::GetInstance()->Get("Bird");
	animator.Initialize();
	animator.PlayAnimation(3, false);
	count = 0;
	fly = true;
}

void ClearBird::Update(float deltaTime)
{
	if (fly && count == 0)
	{
		//飛び立つ
		animator.PlayAnimation(2, false);
		count++;
	}
	else if (fly)
	{
		if (animator.IsEndAnimation())count++;
	}

	if (count != 0 && count % 50 == 0)
	{
		//羽ばたき
		animator.PlayAnimation(0, false);
	}
	float LERP_VALUE = 0.002f * GE::GameSetting::Time::GetGameTime();


	//result = result.Normalize();
	transform->position = GE::Math::Vector3::Lerp(transform->position, target, LERP_VALUE);
	animator.Update(deltaTime);
}

void ClearBird::Draw()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSkinMeshShader");
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	GE::Material material;
	material.color = GE::Color::White();
	animator.SetAnimationData(graphicsDevice, modelMatrix);

	//renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	GE::CameraInfo cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
	cameraInfo.lightMatrix = gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->GetVPMatrix();
	gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->SetDirectionalLightInfo();
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });


	graphicsDevice->DrawMesh("Player");
}