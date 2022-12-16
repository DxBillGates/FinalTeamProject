#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/Graphics/Camera3DDebug.h>

#include"BirdEnemy.h"
#include"PlayerComponent.h"

BirdEnemy::BirdEnemy()
{

}

void BirdEnemy::Awake()
{
	GE::Utility::Printf("BirdEnemy Awake()\n");
}

void BirdEnemy::Start()
{
	GE::Utility::Printf("BirdEnemy Start()\n");
	random = GE::RandomMaker::GetFloat(-1.0f, 1.0f);
	float randPos = GE::RandomMaker::GetFloat(100.0f, 1000.0f);
	transform->position = { 0.0f,randPos,0.0f };//位置をランダム化
	transform->scale = { 100.0f,100.0f,100.0f };//サイズ
	float randSpeed = GE::RandomMaker::GetFloat(10.0f, 25.0f);

	speed = randSpeed;//速度
	statas = Statas::ALIVE;

	gameObject->SetColor(GE::Color::Red());
}

void BirdEnemy::Update(float deltaTime)
{
	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();

	//回転処理
	transform->rotation = GE::Math::Quaternion(GE::Math::Vector3(0.0f, 1.0f, 0.0f), bodyDirection.y);
	bodyDirection.y -= 0.005f * random;

	transform->position += transform->GetForward() * speed * GE::GameSetting::Time::GetGameTime();//移動処理
}

void BirdEnemy::Draw()
{
	if (statas == Statas::DEAD) { return; }

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	//transform->scale = { 200,200,2000 };
	material.color = gameObject->GetColor();
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();


	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("Bird1");
}

void BirdEnemy::LateDraw()
{

}

void BirdEnemy::OnCollision(GE::GameObject* other)
{
	GE::Utility::Printf("BirdEnemy OnCollision(GameObject* other) : hit\n");
	
}

void BirdEnemy::OnCollision(GE::ICollider* hitCollider)
{
	GE::Utility::Printf("BirdEnemy OnCollision(ICollider* other) : hit\n");
}

void BirdEnemy::OnCollisionEnter(GE::GameObject* other)
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

void BirdEnemy::OnGui()
{
	float dragSpeed = 0.1f;
	float maxValue = 100.0f;
	ImGui::DragFloat("Speed", &speed, dragSpeed, 0, maxValue);
}