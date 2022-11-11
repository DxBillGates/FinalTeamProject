#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/Graphics/Camera3DDebug.h>

#include"BirdEnemy.h"

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
	random = { GE::RandomMaker::GetFloat(-1,1),GE::RandomMaker::GetFloat(10,25),GE::RandomMaker::GetFloat(-500.0f,500.0f) };//敵の位置のランダム変数
	transform->position = { 0,GE::RandomMaker::GetFloat(100,1000),0 };//位置をランダム化
	transform->scale = { 50.0f,50.0f,50.0f };

	speed = random.y;
	statas = Statas::ALIVE;

	material.color = GE::Color::Red();
}
void BirdEnemy::Update(float deltaTime)
{
	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();

	transform->rotation = GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), bodyDirection.y);
	bodyDirection.y -= 0.005 * random.x;

	transform->position += transform->GetForward() * speed;
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
	if (statas == Statas::ALIVE)
	{
		statas = Statas::DEAD;
	}
}

void BirdEnemy::OnCollision(GE::ICollider* hitCollider)
{
	GE::Utility::Printf("BirdEnemy OnCollision(ICollider* other) : hit\n");
}

void BirdEnemy::OnGui()
{
	float dragSpeed = 0.1f;
	float maxValue = 100;
	ImGui::DragFloat("Speed", &speed, dragSpeed, 0, maxValue);
}