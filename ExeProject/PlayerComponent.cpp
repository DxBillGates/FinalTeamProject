#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/Graphics/Camera3DDebug.h>

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
	transform->position = { 0,10,0 };
	transform->scale = { 4,2,2 };

	body_direction = { 0,0,0 };
	dashEasingCount = 0.0;
	state = PlayerState::MOVE;

	normal_speed = 1000;
	dash_speed = 10000;
	current_speed = normal_speed;
	gravity = { 0,0.5,0 };
	normal_cameraDistance = 500;
	dash_cameraDistance = 700;
	current_cameraDistance = normal_cameraDistance;
	dir = 0.0;

	cameraC = new CameraControl();
	cameraC->Initialize();
	cameraC->SetGraphicsDevice(graphicsDevice);
	cameraC->SetPosition(transform->position);
}
void PlayerComponent::Update(float deltaTime)
{

	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();
	//‘€ì
	Control(1);

	cameraC->Direction(transform->position);
	cameraC->SetOtherAxis(transform->GetMatrix().GetAxis());

	cameraC->SetPosition(transform->position);
	//axis.z.y‚ªMAX‚É‚È‚Á‚Äz.x‚Æz.z‚ª0‚É‚È‚é‚Ì‚ð–h‚®
	if (abs(axis.z.y) < 0.999)
	{
		dir = atan2f(axis.z.x, axis.z.z);
	}
	cameraC->SetDir(dir);
	cameraC->Update();

	//D0‰Ÿ‚µ‚½‚çˆÚ“®’âŽ~–ƒfƒoƒbƒO—p
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D0))
	{
		state != PlayerState::STOP_DEBUG ? state = PlayerState::STOP_DEBUG : state = PlayerState::MOVE;
	}
	if (state != PlayerState::STOP_DEBUG)transform->position += axis.z * current_speed * deltaTime - gravity;
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
	ImGui::DragFloat("Speed", &current_speed, dragSpeed, 0, maxValue);
	ImGui::DragFloat3("RandomVector", random.value, dragSpeed, -1, 1);
	ImGui::DragFloat3("GyroVector", gyro.value, dragSpeed, -1, 1);
}

void PlayerComponent::Control(float deltaTime)
{
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::RIGHT))
	{
		body_direction.y += 0.01 * deltaTime;
		body_direction.z > -0.3 ? body_direction.z -= 0.005 * deltaTime : 0;

	}
	else if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::LEFT))
	{
		body_direction.y -= 0.01 * deltaTime;
		body_direction.z < 0.3 ? body_direction.z += 0.005 * deltaTime : 0;
	}
	else
	{
		abs(body_direction.z) < 0.005 ? body_direction.z = 0 : body_direction.z > 0.0 ? body_direction.z -= 0.005 * deltaTime : body_direction.z += 0.005 * deltaTime;
	}
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::UP))
	{
		body_direction.x > -1.57 ? body_direction.x -= 0.005 * deltaTime : 0;
	}
	else if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::DOWN))
	{
		body_direction.x < 1.57 ? body_direction.x += 0.005 * deltaTime : 0;
	}
	else
	{
		abs(body_direction.x) < 0.01 ? body_direction.x = 0 : body_direction.x > 0.0 ? body_direction.x -= 0.01 * deltaTime : body_direction.x += 0.01 * deltaTime;
	}
	float dash_time = 100;
	switch (state)
	{
	case PlayerComponent::PlayerState::STOP_DEBUG:
		break;
	case PlayerComponent::PlayerState::MOVE:
		//Key‰Ÿ‚µ‚½‚çPlayerState::DASH‚É•Ï‚í‚é
		if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE))
		{
			state = PlayerState::DASH;
		}
		break;
	case PlayerComponent::PlayerState::DASH:
		//ƒXƒs[ƒh‚Ì‘JˆÚ
		//current_speed = GE::Math::Easing::Lerp(dash_speed, normal_speed, speedEasingCount / time);
		current_speed = easeIn(dash_speed, normal_speed, dashEasingCount / dash_time);
		//ƒJƒƒ‰‚Ì‹——£‘JˆÚA‘JˆÚ‚Å—£‚ê‚Ä‘JˆÚ‚ÅŒ³‚É–ß‚é
		//current_cameraDistance = easeIn(normal_cameraDistance, dash_cameraDistance,
		//	sin(GE::Math::Easing::Lerp(0, 3.14, dashEasingCount / dash_time)));
		cameraC->DashCam(dashEasingCount, dash_time);

		if (dashEasingCount < dash_time) { dashEasingCount++; }
		else { state = PlayerState::MOVE; dashEasingCount = 0.0f; }

		break;
	case PlayerComponent::PlayerState::STAY_LAND:
		break;
	default:
		break;
	}
	transform->rotation =
		GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), body_direction.y)
		* GE::Math::Quaternion(GE::Math::Vector3(0, 0, 1), body_direction.z)
		* GE::Math::Quaternion(GE::Math::Vector3(1, 0, 0), body_direction.x);
}
//EaseInŠÖŒW‚ª‚æ‚­‚í‚©‚ç‚È‚©‚Á‚½‚©‚çˆêŽž“I‚É’Ç‰Á
const float PlayerComponent::easeIn(const float start, const float end, float time)
{
	return start * (1.0f - time * time) + end * time * time;
}
