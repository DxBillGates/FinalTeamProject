#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/GameFramework/Collision/CollisionManager.h>
#include <GatesEngine/Header\GameFramework/GameSetting.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

#include "FieldObjectManager.h"
#include "BirdChild.h"

BirdChild::BirdChild()
{

}

void BirdChild::Awake()
{
	GE::Utility::Printf("BirdChild Awake()\n");
}

void BirdChild::Start()
{
	animator = GE::SkinMeshManager::GetInstance()->Get("hina");
	animator.Initialize();
	animator.PlayAnimation(0, true);
}
void BirdChild::Update(float deltaTime)
{
	animator.Update(deltaTime);
}

void BirdChild::DrawShadow()
{

	/*GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSkinMeshShader");

	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();
	GE::Material material;
	material.color = GE::Color::White();

	animator.SetAnimationData(graphicsDevice, modelMatrix);
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });

	graphicsDevice->DrawMesh("Hina");*/

}

void BirdChild::Draw()
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

void BirdChild::LateDraw()
{

}

void BirdChild::OnCollision(GE::GameObject* other)
{

}

void BirdChild::OnCollisionEnter(GE::GameObject* other)
{

}

void BirdChild::OnCollisionExit(GE::GameObject* other)
{
}

void BirdChild::OnCollision(GE::ICollider* hitCollider)
{
}

void BirdChild::OnGui()
{

}
