#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

#include"NormalEnemy.h"

NormalEnemy::NormalEnemy()
{

}

void NormalEnemy::Awake()
{
	GE::Utility::Printf("NormalEnemy Awake()\n");
}

void NormalEnemy::Start()
{
	GE::Utility::Printf("NormalEnemy Start()\n");
	speed = 10;
	angle = GE::RandomMaker::GetFloat(0.0f, 20.0f);//敵の始動位置の調整
	if (PlayerComponent::dashMode)
	{
		gameObject->SetColor(GE::Color::Green());
	}
	else
	{
		gameObject->SetColor(GE::Color::Red());
	}
	animator = GE::SkinMeshManager::GetInstance()->Get("Dragonfly");
	animator.Initialize();
	animator.PlayAnimation(0, true);

	modelName = "Dragonfly";
	Enemy::Start();

}
void NormalEnemy::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	if (statas != Statas::ALIVE) { return; }
	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();
	float range = 3.0f;//ホバリングの幅
	const float f = 144.0f / (1.0f / deltaTime);

	if (!PlayerComponent::dashMode)
	{
		angle += 0.02f * GE::GameSetting::Time::GetGameTime() * f;//ホバリングの速さ

		transform->rotation = GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), angle);
		transform->position += transform->GetForward() * speed * GE::GameSetting::Time::GetGameTime() * f;
	}
	animator.Update(deltaTime);

}

void NormalEnemy::DrawShadow()
{
	if (statas != Statas::ALIVE) { return; }

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

void NormalEnemy::Draw()
{
	if (statas != Statas::ALIVE) { return; }

	graphicsDevice->SetLayer("effectLayer");
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

void NormalEnemy::LateDraw()
{
}

void NormalEnemy::OnCollision(GE::GameObject* other)
{
}

void NormalEnemy::OnCollision(GE::ICollider* hitCollider)
{
}

void NormalEnemy::OnGui()
{
	float dragSpeed = 0.1f;
	float maxValue = 100;
	ImGui::DragFloat("Speed", &speed, dragSpeed, 0, maxValue);
	ImGui::DragFloat3("RandomVector", random.value, dragSpeed, -1, 1);
}