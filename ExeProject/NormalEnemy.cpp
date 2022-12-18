#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >

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
	random = { GE::RandomMaker::GetFloat(-1000.0f,1000.0f),GE::RandomMaker::GetFloat(0.0f,1000.0f),GE::RandomMaker::GetFloat(-500.0f,500.0f) };//敵の位置のランダム変数
	transform->position = random;//位置をランダム化
	transform->scale = { 100.0f,100.0f,100.0f };
	speed = 10;
	angle = GE::RandomMaker::GetFloat(0.0f, 20.0f);//敵の始動位置の調整
	gameObject->SetColor(GE::Color::Red());
}
void NormalEnemy::Update(float deltaTime)
{
	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();
	float range = 1.0f;//ホバリングの幅
	angle += 1.0 * GE::GameSetting::Time::GetGameTime() * deltaTime;//ホバリングの速さ
	transform->position = (transform->position + GE::Math::Vector3(0.0f, sinf(angle) * range * GE::GameSetting::Time::GetGameTime(), 0.0f));
}

void NormalEnemy::Draw()
{
	if (statas == Statas::DEAD) { return; }
	
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	GE::Material material;
	material.color = gameObject->GetColor();

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
		if (PlayerComponent::IsSpeedy())//一定上の速度
		{
			statas = Statas::DEAD;
			gameObject->Destroy();
		}
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