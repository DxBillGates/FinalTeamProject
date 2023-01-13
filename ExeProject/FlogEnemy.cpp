#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/Graphics/Camera3DDebug.h>

#include "FlogEnemy.h"

#include"PlayerComponent.h"

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
	transform->scale = { 100.0f,100.0f,100.0f };//サイズ
	float randSpeed = GE::RandomMaker::GetFloat(10.0f, 25.0f);

	speed = randSpeed;//速度
	statas = Statas::ALIVE;

	accelerate = { 0,15,10 };
	gravity = { 0,-0.2f,0 };

	gameObject->SetColor(GE::Color::Red());
}

void FlogEnemy::Update(float deltaTime)
{
	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();

	if (transform->position.y <= 0)//地面についてたら
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
		velocity += accelerate;
		jumpCount = 0;
	}

	transform->position += velocity;//移動処理

	//transform->position += transform->GetForward() * speed * GE::GameSetting::Time::GetGameTime();//移動処理
}

void FlogEnemy::Draw()
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
