#include <GatesEngine/Header/Graphics\CBufferStruct.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/Graphics/Camera3DDebug.h>

#include"PlayerComponent.h"
#include"EnemyManager.h"
#include"NormalEnemy.h"

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
	transform->position = { 0,10,0 };
	transform->scale = { 50,50,50 };

	body_direction = { 0,0,0 };
	dashEasingCount = 0.0;
	statas = PlayerStatas::MOVE;

	normal_speed = 1000;
	current_speed = normal_speed;
	gravity = { 0,0.5,0 };
	dir = 0.0;

	isLockOn = false;
	CameraControl::GetInstance()->Initialize();
	CameraControl::GetInstance()->SetGraphicsDevice(graphicsDevice);
	CameraControl::GetInstance()->SetOtherPos(transform->position);
}
void PlayerComponent::Update(float deltaTime)
{

	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();
	//‘€ì

	Control(deltaTime, 1);

	CameraControl::GetInstance()->Direction(transform->position);
	CameraControl::GetInstance()->SetOtherAxis(transform->GetMatrix().GetAxis());
	CameraControl::GetInstance()->SetOtherPos(transform->position);
	//axis.z.y‚ªMAX‚É‚È‚Á‚Äz.x‚Æz.z‚ª0‚É‚È‚é‚Ì‚ð–h‚®
	CameraControl::GetInstance()->SetDir(dir);
	CameraControl::GetInstance()->Update();

	//D0‰Ÿ‚µ‚½‚çˆÚ“®’âŽ~–ƒfƒoƒbƒO—p
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D0))
	{
		statas != PlayerStatas::STOP_DEBUG ? statas = PlayerStatas::STOP_DEBUG : statas = PlayerStatas::MOVE;
	}
}

void PlayerComponent::Draw()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();
	GE::Material material;
	material.color = GE::Color::White();

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	graphicsDevice->DrawMesh("Bird1");
}

void PlayerComponent::LateDraw()
{

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
	ImGui::DragFloat3("GyroVector", gyro.value, dragSpeed, -1, 1);
}

void PlayerComponent::Control(float deltaTime, float gameTime)
{

	GE::Math::Vector3 c;
	GE::Math::Vector3 up = { 0,1,0 };
	GE::Math::Matrix4x4 matrix;
	switch (statas)
	{
	case PlayerComponent::PlayerStatas::STOP_DEBUG:
		//‰ñ“]
		transform->rotation =
			GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), body_direction.y)
			* GE::Math::Quaternion(GE::Math::Vector3(0, 0, 1), body_direction.z)
			* GE::Math::Quaternion(GE::Math::Vector3(1, 0, 0), body_direction.x);
		break;
	case PlayerComponent::PlayerStatas::MOVE:
		//Key‰Ÿ‚µ‚½‚çPlayerState::DASH‚É•Ï‚í‚é
		if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE)) { statas = PlayerStatas::DASH; }

		transform->position += transform->GetForward() * current_speed * deltaTime * gameTime - gravity;
		//‰ñ“]
		transform->rotation =
			GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), body_direction.y)
			* GE::Math::Quaternion(GE::Math::Vector3(0, 0, 1), body_direction.z)
			* GE::Math::Quaternion(GE::Math::Vector3(1, 0, 0), body_direction.x);
		//ƒƒbƒNƒIƒ“‚µ‚ÄUŒ‚
		LockOn();
		break;
	case PlayerComponent::PlayerStatas::DASH:

		Dash(10000, 100.0, deltaTime, gameTime);

		break;
	case PlayerComponent::PlayerStatas::LOCKON_SHOOT:

		if (lockOnEnemy.object->GetComponent<NormalEnemy>()->statas != NormalEnemy::Statas::DEAD)
		{
			lockOnEnemy.direction = GE::Math::Vector3(lockOnEnemy.object->GetTransform()->position - transform->position).Normalize();
		}
		else { isLockOn = false; }

		//ƒxƒNƒgƒ‹‚Ì•ûŒü‚É‘Ì‚ðŒü‚¯‚é
		c = GE::Math::Vector3::Cross(lockOnEnemy.direction, up);
		matrix = {
			up.x, up.y, up.z, 0.0f,
			c.x, c.y, c.z, 0.0f,
			lockOnEnemy.direction.x, lockOnEnemy.direction.y, lockOnEnemy.direction.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };
		transform->rotation = GE::Math::Quaternion(matrix);

		Dash(5000, 50, deltaTime, gameTime);
		break;
	case PlayerComponent::PlayerStatas::STAY_LAND:
		break;
	default:
		break;
	}
	//‘Ì‚ÌŠp“xŒvŽZ
	if (abs(transform->GetForward().y) < 0.6)
	{
		dir = atan2f(transform->GetForward().x, transform->GetForward().z);
	}

	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::RIGHT))
	{
		body_direction.y += 0.01 * gameTime;
		body_direction.z > -0.3 ? body_direction.z -= 0.005 * gameTime : 0;
	}
	else if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::LEFT))
	{
		body_direction.y -= 0.01 * gameTime;
		body_direction.z < 0.3 ? body_direction.z += 0.005 * gameTime : 0;
	}
	else
	{
		abs(body_direction.z) < 0.005 ? body_direction.z = 0 : body_direction.z > 0.0 ? body_direction.z -= 0.005 * gameTime : body_direction.z += 0.005 * gameTime;
	}
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::UP))
	{
		body_direction.x > -1.57 ? body_direction.x -= 0.005 * gameTime : 0;
	}
	else if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::DOWN))
	{
		body_direction.x < 1.57 ? body_direction.x += 0.005 * gameTime : 0;
	}
	else
	{
		abs(body_direction.x) < 0.01 ? body_direction.x = 0 : body_direction.x > 0.0 ? body_direction.x -= 0.01 * gameTime : body_direction.x += 0.01 * gameTime;
	}
}
void PlayerComponent::LockOn()
{
	std::vector<GE::GameObject*> enemies = EnemyManager::GetInstance()->GetNormalEnemies();
	float result = 100000;
	int a = 0;
	bool look = false;

	//Key‰Ÿ‚µ‚½‚çLockOn€”õ
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::RETURN))
	{
		isLockOn = true;
	}
	else
	{
		if (isLockOn && lockOnEnemy.object != nullptr) { statas = PlayerStatas::LOCKON_SHOOT; }
		return;
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		float distance = abs(GE::Math::Vector3::Distance(transform->position, enemies[i]->GetTransform()->position));
		GE::Math::Vector3 enemyDirection = enemies[i]->GetTransform()->position - transform->position;
		enemies[i]->GetComponent<NormalEnemy>()->SetColor(GE::Color::Red());
		//¶‚«‚Ä‚¢‚é‚©•‘O‘¤‚É‚¢‚é’†‚ÅÅ‚à‹ß‚¢“G
		if (enemies[i]->GetComponent<NormalEnemy>()->statas != NormalEnemy::Statas::DEAD)
		{
			if (GE::Math::Vector3::Dot(transform->GetForward(), enemyDirection.Normalize()) > 0.8)
			{
				look = true;
				if (result > distance)
				{
					result = distance;
					a = i;
				}
			}
		}
	}
	if (look)
	{
		enemies[a]->GetComponent<NormalEnemy>()->SetColor(GE::Color::Blue());
		lockOnEnemy.object = enemies[a];
	}
}
//EaseInŠÖŒW‚ª‚æ‚­‚í‚©‚ç‚È‚©‚Á‚½‚©‚çˆêŽž“I‚É’Ç‰Á
const float PlayerComponent::easeIn(const float start, const float end, float time)
{
	return start * (1.0f - time * time) + end * time * time;
}

void PlayerComponent::Dash(float dash_speed, float dash_time, float deltaTime, float gameTime)
{
	//ƒXƒs[ƒh‚Ì‘JˆÚ
	current_speed = easeIn(dash_speed, normal_speed, dashEasingCount / dash_time);
	CameraControl::GetInstance()->DashCam(dashEasingCount, dash_time);

	if (dashEasingCount < dash_time) { dashEasingCount++; }
	else { statas = PlayerStatas::MOVE; dashEasingCount = 0.0f; }

	transform->position += transform->GetForward() * current_speed * deltaTime * gameTime;
}
