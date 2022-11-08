#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/Graphics/Camera3DDebug.h>

#include"NormalEnemy.h"

float NormalEnemy::gameTime = 1.0;
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
	random = { GE::RandomMaker::GetFloat(-1000.0f,1000.0f),GE::RandomMaker::GetFloat(0.0f,1000.0f),GE::RandomMaker::GetFloat(-500.0f,500.0f) };//敵の位置のランダム変数
	transform->position = random;//位置をランダム化
	transform->scale = { 100.0f,100.0f,100.0f };
	speed = 10;
	statas = Statas::ALIVE;
	angle = GE::RandomMaker::GetFloat(0.0f, 20.0f);//敵の始動位置の調整

	material.color = GE::Color::Red();
}
void NormalEnemy::Update(float deltaTime)
{
	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();
	float range = 1.0f;//ホバリングの幅
	angle += 1.0 * gameTime * deltaTime;//ホバリングの速さ
	transform->position = (transform->position + GE::Math::Vector3(0.0f, sinf(angle) * range * gameTime, 0.0f));
}

void NormalEnemy::Draw()
{
	if (statas == Statas::DEAD) { return; }

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	//transform->scale = { 200,200,2000 };

	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();


	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("Sphere");
}

void NormalEnemy::LateDraw()
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

void NormalEnemy::OnCollision(GE::GameObject* other)
{
	GE::Utility::Printf("NormalEnemy OnCollision(GameObject* other) : hit\n");
	if (statas == Statas::ALIVE)
	{
		statas = Statas::DEAD;
	}
}

void NormalEnemy::OnCollision(GE::ICollider* hitCollider)
{
	GE::Utility::Printf("NormalEnemy OnCollision(ICollider* other) : hit\n");
}

void NormalEnemy::OnGui()
{
	float dragSpeed = 0.1f;
	float maxValue = 100;
	ImGui::DragFloat("Speed", &speed, dragSpeed, 0, maxValue);
	ImGui::DragFloat3("RandomVector", random.value, dragSpeed, -1, 1);
}

void NormalEnemy::SetColor(GE::Color color)
{
	material.color = color;
}
