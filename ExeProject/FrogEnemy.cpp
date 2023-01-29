#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/Graphics/Camera3DDebug.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

#include "FrogEnemy.h"

#include"PlayerComponent.h"
#include"FieldObjectManager.h"

FrogEnemy::FrogEnemy()
{

}

void FrogEnemy::Awake()
{
	GE::Utility::Printf("FrogEnemy Awake()\n");
}

void FrogEnemy::Start()
{
	GE::Utility::Printf("FrogEnemy Start()\n");
	float randSpeed = GE::RandomMaker::GetFloat(10.0f, 25.0f);

	animator = GE::SkinMeshManager::GetInstance()->Get("Frog");
	animator.Initialize();

	speed = randSpeed;//‘¬“x

	accelerate = { 30 };
	gravity = { 0,-2.0f,0 };
	state = 0;
	jumpCount = GE::RandomMaker::GetFloat(1.0f, 90.0f);

	gameObject->SetColor(GE::Color::Red());

	modelName = "Frog";
}

void FrogEnemy::Update(float deltaTime)
{
	const float frameRate = 1.0f / deltaTime;
	const float f = 144.0f / frameRate;

	animator.Update(deltaTime);

	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();
	const int jumpInterval = 180;
	switch (frogState)
	{
	case FrogState::STAY:
		//ƒWƒƒƒ“ƒv
		if (jumpCount > jumpInterval)
		{
			animator.PlayAnimation(4, false);
			velocity += accelerate;
			jumpCount = 0.0f;
			frogState = FrogState::JUMPING;
			currentAngle = angle;
		}
		jumpCount += f * GE::GameSetting::Time::GetGameTime();
		angle = GE::Math::Lerp(currentAngle, currentAngle + 90, jumpCount / (float)jumpInterval);

		break;
	case FrogState::WALK:
		if (animator.IsEndAnimation())
		{
			animator.PlayAnimation(0, true);
			frogState = FrogState::STAY;
		}
		break;
	case FrogState::JUMPING:
		velocity += gravity * GE::GameSetting::Time::GetGameTime();
		break;
	}
	transform->rotation = GE::Math::Quaternion::Euler({ 0,angle,0 });
	transform->position += GE::Math::Vector3(transform->GetForward().x * velocity.x, velocity.y, transform->GetForward().z * velocity.z) * f * GE::GameSetting::Time::GetGameTime();
}

void FrogEnemy::DrawShadow()
{
	if (statas == Statas::DEAD) { return; }

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSkinMeshShader");

	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();
	GE::Material material;
	material.color = GE::Color::White();

	animator.SetAnimationData(graphicsDevice, modelMatrix);
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });

	graphicsDevice->DrawMesh(modelName);
}

void FrogEnemy::Draw()
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
	graphicsDevice->DrawMesh(modelName);
}

void FrogEnemy::LateDraw()
{

}

void FrogEnemy::OnCollision(GE::GameObject* other)
{

}

void FrogEnemy::OnCollision(GE::ICollider* hitCollider)
{
}

void FrogEnemy::OnCollisionEnter(GE::GameObject* other)
{
	if (statas == Statas::ALIVE)
	{
		if (other->GetTag() == "tile")
		{
			if (frogState == FrogState::JUMPING)
			{
				frogState = FrogState::WALK;
				animator.PlayAnimation(3, false);
				velocity = {};
			}
		}
	}
	Enemy::OnCollisionEnter(other);
}

void FrogEnemy::OnGui()
{
	float dragSpeed = 0.1f;
	float maxValue = 100.0f;
	ImGui::DragFloat("Speed", &speed, dragSpeed, 0, maxValue);
}
