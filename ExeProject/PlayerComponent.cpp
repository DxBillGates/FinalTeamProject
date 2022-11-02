#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >

#include"PlayerComponent.h"

PlayerComponent::PlayerComponent()
	: inputDevice(nullptr)
{

}

void PlayerComponent::Awake()
{
	GE::Utility::Printf("PlayerComponent Awake()\n");
}

void PlayerComponent::Start()
{
	GE::Utility::Printf("PlayerComponent Start()\n");
	inputDevice = GE::InputDevice::GetInstance();
	random = { GE::RandomMaker::GetFloat(-1,1),GE::RandomMaker::GetFloat(-1,1),GE::RandomMaker::GetFloat(-1,1) };
	speed = 1000;
	transform->position = { 0,-10,100 };
	transform->scale = { 4,2,2 };
	move = { 0,0,-0 };

	gravity = { 0,0.5,0 };

	stop_move = false;
}
void PlayerComponent::Update(float deltaTime)
{

	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();
	//‘€ì
	Control();

	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D0))
	{
		stop_move == true ? stop_move = false : stop_move = true;
	}
	if (!stop_move)transform->position += axis.z * speed * deltaTime - gravity;
}

void PlayerComponent::Draw()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	transform->scale = { 200,50,50 };

	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();
	GE::Material material;
	material.color = GE::Color::White();

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("Cube");
}

void PlayerComponent::LateDraw()
{
	const float SPRITE_SIZE = 100;

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSpriteShader");

	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ SPRITE_SIZE });
	GE::Math::Vector2 mousePos = inputDevice->GetMouse()->GetClientMousePos();
	//GE::Utility::Printf("%d,%d\n",(int)mousePos.x, (int)mousePos.y);

	modelMatrix *= GE::Math::Matrix4x4::Translate({ mousePos.x,mousePos.y,0 });
	GE::Material material;
	material.color = GE::Color::White();

	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,1,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("2DPlane");
}

void PlayerComponent::OnCollision(GE::GameObject* other)
{
	GE::Utility::Printf("PlayerComponent OnCollision(GameObject* other) : hit\n");
}

void PlayerComponent::OnCollision(GE::ICollider* hitCollider)
{
	GE::Utility::Printf("PlayerComponent OnCollision(ICollider* other) : hit\n");
}

void PlayerComponent::OnGui()
{
	float dragSpeed = 0.1f;
	float maxValue = 100;
	ImGui::DragFloat("Speed", &speed, dragSpeed, 0, maxValue);
	ImGui::DragFloat3("RandomVector", random.value, dragSpeed, -1, 1);
	ImGui::DragFloat3("GyroVector", gyro.value, dragSpeed, -1, 1);
}

void PlayerComponent::Control()
{
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::RIGHT))
	{
		move.y += 0.01;
		move.z > -0.3 ? move.z -= 0.005 : 0;

	}
	else if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::LEFT))
	{
		move.y -= 0.01;
		move.z < 0.3 ? move.z += 0.005 : 0;
	}
	else
	{
		move.z == 0.0 ? 0 : move.z > 0.0 ? move.z -= 0.005 : move.z += 0.005;
	}
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::UP))
	{
		move.x > -1.57 ? move.x -= 0.005 : 0;
	}
	else if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::DOWN))
	{
		move.x < 1.57 ? move.x += 0.005 : 0;
	}
	else
	{
		move.x == 0.0 ? 0 : move.x > 0.0 ? move.x -= 0.01 : move.x += 0.01;
	}

	transform->rotation =
		GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), move.y)
		* GE::Math::Quaternion(GE::Math::Vector3(0, 0, 1), move.z)
		* GE::Math::Quaternion(GE::Math::Vector3(1, 0, 0), move.x);
}
