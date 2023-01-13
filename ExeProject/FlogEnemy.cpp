#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/Graphics/Camera3DDebug.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

#include "FlogEnemy.h"

#include"PlayerComponent.h"
#include"FieldObjectManager.h"

FlogEnemy::FlogEnemy()
{

}

void FlogEnemy::Awake()
{
	GE::Utility::Printf("FlogEnemy Awake()\n");
}

void FlogEnemy::Start()
{
	GE::Utility::Printf("FlogEnemy Start()\n");
	random = GE::RandomMaker::GetFloat(-1.0f, 1.0f);
	float randPos = GE::RandomMaker::GetFloat(100.0f, 1000.0f);
	transform->position = { 0.0f,0.0f,0.0f };//位置をランダム化
	transform->scale = { 10 };//サイズ
	float randSpeed = GE::RandomMaker::GetFloat(10.0f, 25.0f);

	animator = GE::SkinMeshManager::GetInstance()->Get("Frog");
	animator.Initialize();

	speed = randSpeed;//速度
	statas = Statas::ALIVE;

	accelerate = { 15,15,15 };
	gravity = { 0,-0.5f,0 };
	state = 0;
	jumpCount = 1;

	gameObject->SetColor(GE::Color::Red());
}

void FlogEnemy::Update(float deltaTime)
{
	animator.Update(deltaTime);

	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();

	//着地
	if (jumpCount == 0)
	{
		animator.PlayAnimation(2, false);
	}
	if (jumpCount == 0 && transform->position.y <= 0)
	{
		angle += 90;
	}
	//地面についてたら
	if (transform->position.y <= 0)
	{
		velocity = {};
		jumpCount++;
	}
	else
	{
		velocity += gravity;
	}

	//ジャンプ
	if (jumpCount == 180)
	{
		animator.PlayAnimation(0, false);
		velocity += accelerate;
		jumpCount = 0;
	}

	transform->rotation = GE::Math::Quaternion::Euler({ 0,angle,0 });
	transform->position += GE::Math::Vector3(transform->GetForward().x * velocity.x,velocity.y, transform->GetForward().z * velocity.z);
}

void FlogEnemy::DrawShadow()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

graphicsDevice->SetShader("DefaultSkinMeshShader");

GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();
GE::Material material;
material.color = GE::Color::White();

animator.SetAnimationData(graphicsDevice, modelMatrix);
renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });

graphicsDevice->DrawMesh("Frog");
}

void FlogEnemy::Draw()
{
	if (statas == Statas::DEAD) { return; }

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSkinMeshShader");

	//transform->scale = { 200,200,2000 };
	material.color = gameObject->GetColor();
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();
	animator.SetAnimationData(graphicsDevice, modelMatrix);

	GE::CameraInfo cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
	cameraInfo.lightMatrix = gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->GetVPMatrix();
	gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->SetDirectionalLightInfo();
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });
	graphicsDevice->DrawMesh("Frog");
}

void FlogEnemy::LateDraw()
{

}

void FlogEnemy::OnCollision(GE::GameObject* other)
{
	GE::Utility::Printf("FlogEnemy OnCollision(GameObject* other) : hit\n");

}

void FlogEnemy::OnCollision(GE::ICollider* hitCollider)
{
	GE::Utility::Printf("FlogEnemy OnCollision(ICollider* other) : hit\n");
}

void FlogEnemy::OnCollisionEnter(GE::GameObject* other)
{
	if (statas == Statas::ALIVE)
	{
		if (PlayerComponent::IsSpeedy())//一定上の速度
		{
			statas = Statas::DEAD;
			gameObject->Destroy();
		}
	}
}

void FlogEnemy::OnGui()
{
	float dragSpeed = 0.1f;
	float maxValue = 100.0f;
	ImGui::DragFloat("Speed", &speed, dragSpeed, 0, maxValue);
}
