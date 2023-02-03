#include "OverChild.h"
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header\GameFramework/GameSetting.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

void OverChild::Awake()
{
}

void OverChild::Start()
{
	animator = GE::SkinMeshManager::GetInstance()->Get("hina");
	animator.Initialize();
	animator.PlayAnimation(0, false);


	gravity = -2.0f;
}

void OverChild::Update(float deltaTime)
{
	animator.Update(deltaTime);

	//if (transform->position.y >= 200)
	//{//‹ó’†
	//	transform->position.y = transform->position.y + gravity;
	//	transform->rotation *= GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), 0.2f * deltaTime * GE::GameSetting::Time::GetGameTime());
	//}
	//else
	//{//’n–Ê
	//	transform->rotation = GE::Math::Quaternion::Euler({ -90,0,0 });
	//}
}

void OverChild::Draw()
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

	graphicsDevice->DrawMesh("Hina");
}
