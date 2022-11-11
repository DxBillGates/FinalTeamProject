#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/Graphics/Camera3DDebug.h>

#include"BirdEnemy.h"

float BirdEnemy::GameTime = 1.0;
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
	random = { GE::RandomMaker::GetFloat(-1000.0f,1000.0f),GE::RandomMaker::GetFloat(0.0f,1000.0f),GE::RandomMaker::GetFloat(-500.0f,500.0f) };//敵の位置のランダム変数
	transform->position = { 100,100,0 };//位置をランダム化
	transform->scale = { 100.0f,100.0f,100.0f };
	speed = 0.03f;
	statas = Statas::ALIVE;
	
	material.color = GE::Color::Red();
}
void BirdEnemy::Update(float deltaTime)
{
	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();

	transform->rotation = GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), bodyDirection.y);
	bodyDirection.y -= 0.01;

	transform->position += transform->GetForward() * 10;
}

void BirdEnemy::Draw()
{
	//if (statas == Statas::DEAD) { return; }

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	//transform->scale = { 200,200,2000 };

	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();


	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("Bird1");
}

void BirdEnemy::LateDraw()
{
	//const float SPRITE_SIZE = 100;

	//GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	//GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	//graphicsDevice->SetShader("DefaultSpriteShader");

	//GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ SPRITE_SIZE });
	//GE::Math::Vector2 mousePos = inputDevice->GetMouse()->GetClientMousePos();
	////GE::Utility::Printf("%d,%d\n",(int)mousePos.x, (int)mousePos.y);

	//modelMatrix *= GE::Math::Matrix4x4::Translate({ mousePos.x,mousePos.y,0 });
	//GE::Material material;
	//material.color = GE::Color::White();

	//GE::CameraInfo cameraInfo;
	//cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,1,0 }, { 0,0,1 }, { 0,1,0 });
	//cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	//renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	//renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	//renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	//graphicsDevice->DrawMesh("2DPlane");
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
	ImGui::DragFloat3("RandomVector", random.value, dragSpeed, -1, 1);
}

void BirdEnemy::SetColor(GE::Color color)
{
	material.color = color;
}
