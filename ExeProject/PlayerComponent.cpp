#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/GameFramework/Collision/CollisionManager.h>

#include"PlayerComponent.h"
#include"EnemyManager.h"
#include "InputManager.h"
#include "CameraControl.h"

float PlayerComponent::GameTime = 1.0;
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
	transform->position = { 0,500,-5000 };
	transform->scale = { 50,50,50 };

	body_direction = { 0,0,0 };
	dashEasingCount = 0.0;
	statas = PlayerStatas::MOVE;

	normal_speed = 1000;
	current_speed = normal_speed;
	gravity = { 0,0.5,0 };
	dir = 0.0;
	rayHitSecond = 144.0;

	// ヒットストップの長さ
	hitStopTime = 20.0;
	hitStopCount = hitStopTime;

	//姿勢遷移
	body_direction_LerpTime = 50.0;
	body_direction_LerpCount = body_direction_LerpTime;

	//レティクルの位置
	center = GE::Window::GetWindowSize() / 2.0 + GE::Math::Vector2(0, -100);

	is_rayCast_active = false;

	CameraControl::GetInstance()->Initialize();
	CameraControl::GetInstance()->SetGraphicsDevice(graphicsDevice);
	CameraControl::GetInstance()->SetOtherPos(transform->position);

	InputManager::GetInstance()->Initialize();
}
void PlayerComponent::Update(float deltaTime)
{
	InputManager::GetInstance()->Update();
	const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
	GE::Math::GetScreenToRay(center, &rayPos, &rayDir, cameraInfo.viewMatrix, cameraInfo.projMatrix, GE::Math::Matrix4x4::GetViewportMatrix(GE::Window::GetWindowSize()));

	//ヒットストップのカウント
	if (!InputManager::GetInstance()->GetLockonButton())
	{
		if (hitStopCount < hitStopTime)
		{
			GameTime = 0.01;
			hitStopCount++;
		}
		else
		{
			//GameTime = 0.5;
			GameTime = 1.0;
		}
	}

	const GE::Math::Axis& axis = transform->GetMatrix().GetAxis();
	//操作

	Control(deltaTime);

	NormalEnemy::GameTime = GameTime;

	CameraControl::GetInstance()->Direction(transform->position);
	CameraControl::GetInstance()->SetOtherAxis(transform->GetMatrix().GetAxis());
	CameraControl::GetInstance()->SetOtherPos(transform->position);
	CameraControl::GetInstance()->SetDir(dir);
	CameraControl::GetInstance()->Update();

	//D0押したら移動停止＊デバッグ用
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
	if (!is_rayCast_active)
	{
		return;
	}
	const float SPRITE_SIZE = 30;

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSpriteShader");

	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ GE::Math::Lerp(SPRITE_SIZE,10,rayHitCount / rayHitSecond) });

	modelMatrix *= GE::Math::Matrix4x4::RotationZ(rayHitCount);
	modelMatrix *= GE::Math::Matrix4x4::Translate({ center.x,center.y,0 });
	GE::Material material;
	material.color = gameObject->GetColor();

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
	hitStopCount = 0;
	CameraControl::GetInstance()->ShakeStart({ 50,50 }, 30);

	if (other == lockOnEnemy.object)
	{
		lockOnEnemy.object = nullptr;
	}
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
	ImGui::InputFloat4("quat", quat.value);
	ImGui::InputFloat3("accelerometer", accelerometer.value);

	GE::Math::Vector3 inputAxis = InputManager::GetInstance()->GetAxis();
	ImGui::InputFloat3("inputAxis", inputAxis.value);
}
void PlayerComponent::Control(float deltaTime)
{
	GE::Math::Quaternion BODY_DIRECTION =
		GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), body_direction.y)
		* GE::Math::Quaternion(GE::Math::Vector3(0, 0, 1), body_direction.z)
		* GE::Math::Quaternion(GE::Math::Vector3(1, 0, 0), body_direction.x);

	bool loop = false;
	switch (statas)
	{
	case PlayerComponent::PlayerStatas::STOP_DEBUG:

		//回転
		transform->rotation = BODY_DIRECTION;
		break;
	case PlayerComponent::PlayerStatas::MOVE:

		transform->position += transform->GetForward() * current_speed * deltaTime * GameTime - gravity;
		//Key押したらPlayerState::DASHに変わる
		if (InputManager::GetInstance()->GetActionButton()) { statas = PlayerStatas::DASH; }

		//ダッシュ後体の角度の遷移
		if (body_direction_LerpCount < body_direction_LerpTime)
		{
			body_direction_LerpCount += 1 * GameTime;
			transform->rotation = GE::Math::Quaternion::Lerp(body_direction_LockOn, BODY_DIRECTION, body_direction_LerpCount / body_direction_LerpTime);
		}
		else { transform->rotation = BODY_DIRECTION; }

		if (InputManager::GetInstance()->GetLockonButton())
		{
			isLockOnStart = true;
			//最も近くて前方にいる敵をセット
			SearchNearEnemy();
		}
		else {
			isLockOnStart = false;
		}

		is_rayCast_active = false;
		//RayCast(deltaTime);
		//ロックオンして攻撃
		LockOn();

		break;
	case PlayerComponent::PlayerStatas::DASH:

		Dash(10000, 100.0, deltaTime, transform->GetForward());

		break;
	case PlayerComponent::PlayerStatas::LOCKON_SHOOT:

		if (lockOnEnemy.object != nullptr)
		{
			if (lockOnEnemy.object->GetComponent<Enemy>()->statas != Enemy::Statas::DEAD)
			{
				lockOnEnemy.direction = GE::Math::Vector3(lockOnEnemy.object->GetTransform()->position - transform->position).Normalize();
				loop = true;
			}
			else { isLockOn = false; }
		}

		Dash(10000, 200, deltaTime, lockOnEnemy.direction, loop);
		break;
	case PlayerComponent::PlayerStatas::STAY_LAND:
		break;
	default:
		break;
	}
	//体の角度計算
	if (abs(transform->GetForward().y) < 0.6)
	{
		dir = atan2f(transform->GetForward().x, transform->GetForward().z);
	}

	//キーボードで移動操作
	KeyboardMoveControl();

	if (statas == PlayerStatas::MOVE && accelerometer.Length() > 2.f)
	{
		//GE::Utility::Printf("%f\n", accelerometer.Length());
		statas = PlayerStatas::DASH;
	}
}
void PlayerComponent::KeyboardMoveControl()
{
	GE::Math::Vector3 inputAxis = InputManager::GetInstance()->GetAxis(0, InputManager::InputCtrlAxisState::GYROSCOPE);

	if (inputAxis.x != 0)
	{
		body_direction.y += 0.01 * inputAxis.x * GameTime;
		body_direction.z -= 0.005 * inputAxis.x * GameTime;

		body_direction.z = (body_direction.z > 0.3f || body_direction.z < -0.3) ? 0.3f * ((body_direction.z > 0) ? 1 : -1) : body_direction.z;
	}
	else
	{
		abs(body_direction.z) < 0.005 ? body_direction.z = 0 : body_direction.z > 0.0 ? body_direction.z -= 0.005 * GameTime : body_direction.z += 0.005 * GameTime;
	}

	if (inputAxis.y != 0)
	{
		body_direction.x -= 0.005 * inputAxis.y * GameTime;
		body_direction.x = (body_direction.x > 1.57f || body_direction.x < -1.57f) ? 1.57f * ((body_direction.x > 0) ? 1 : -1) : body_direction.x;
	}
	else
	{
		abs(body_direction.x) < 0.01 ? body_direction.x = 0 : body_direction.x > 0.0 ? body_direction.x -= 0.01 * GameTime : body_direction.x += 0.01 * GameTime;
	}

	// ジョイコン操作中の際の姿勢制御

	GE::Joycon* joycon = inputDevice->GetJoyconL();
	if (joycon == nullptr)return;
	GE::Vector3Int16 gyroData = joycon->GetGyroscope();
	gyro = { (float)gyroData.y,(float)-gyroData.z,(float)-gyroData.x };

	// コントローラーから姿勢を更新し続ける
	quat *= GE::Math::Quaternion(gyro.Normalize(), GE::Math::ConvertToRadian(gyro.Length() * 1.f / 144.f));

	if (InputManager::GetInstance()->GetCurrentInputDeviceState() != InputManager::InputDeviceState::JOYCON)return;
	const float GYRO_OFFSET = 0.05f;
	GE::Math::Vector3 quatVector =
	{
		quat.x,
		quat.y,
		quat.z,
	};

	body_direction.x += quatVector.x / 20.f;
	body_direction.y += quatVector.y / 20.f;
	body_direction.z += quatVector.z / 20.f;

	GE::Math::Vector3 bodyDirectionMax;
	bodyDirectionMax = { 1.0f,100000,0.75f };
	body_direction = GE::Math::Vector3::Min(-bodyDirectionMax, GE::Math::Vector3::Max(bodyDirectionMax, body_direction));
}
void PlayerComponent::SearchNearEnemy()
{
	std::vector<GE::GameObject*> enemies = EnemyManager::GetInstance()->GetAllEnemies();
	float result = 100000;
	int a = 0;
	bool look = false;
	for (int i = 0; i < enemies.size(); i++)
	{
		float distance = abs(GE::Math::Vector3::Distance(transform->position, enemies[i]->GetTransform()->position));
		GE::Math::Vector3 enemyDirection = enemies[i]->GetTransform()->position - transform->position;
		//色初期化
		enemies[i]->SetColor(GE::Color::Red());
		//生きているか＆前側にいる中で最も近い敵
		if (enemies[i]->GetComponent<Enemy>()->statas != Enemy::Statas::DEAD)
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
		//スロー
		GameTime = 0.2;
		//最も近い敵
		lockOnEnemy.object = enemies[a];
		//ロックオンしている敵を青くする
		lockOnEnemy.object->SetColor(GE::Color::Blue());
	}
}
void PlayerComponent::LockOn()
{
	if (lockOnEnemy.object != nullptr
		&& lockOnEnemy.object->GetComponent<Enemy>()->statas != Enemy::Statas::DEAD)
	{
		//Key押したらLockOn準備
		if (isLockOnStart)
		{
			isLockOn = true;
		}
		else
		{
			statas = PlayerStatas::LOCKON_SHOOT;
		}
	}
}

void PlayerComponent::Dash(float dash_speed, float dash_time, float deltaTime, GE::Math::Vector3 direction, bool loop)
{
	if (loop)
	{
		current_speed = dash_speed;
		dashEasingCount = 0.0f;
		body_direction_LerpCount = 0;
	}
	else
	{
		current_speed = easeIn(dash_speed, normal_speed, dashEasingCount / dash_time);

	}
	if (dashEasingCount < dash_time) { dashEasingCount += 1 * GameTime; }
	else { statas = PlayerStatas::MOVE; dashEasingCount = 0.0f; body_direction_LerpCount = 0; }

	//スピードの遷移
	CameraControl::GetInstance()->DashCam(dashEasingCount, dash_time);
	GE::Math::Vector3 forward = { 0,0,1 };
	float dot;
	float theta;
	dot = GE::Math::Vector3::Dot(forward, direction);
	theta = std::acosf(dot);
	GE::Math::Vector3 cross = GE::Math::Vector3::Cross(forward, direction).Normalize();
	body_direction_LockOn = { cross,theta };
	transform->rotation = body_direction_LockOn;

	transform->position += transform->GetForward() * current_speed * deltaTime * GameTime;
}
//CollisionDetectionに引っ越すかも
void PlayerComponent::RayCast(float deltaTime)
{
	is_rayCast_active = true;

	std::vector<GE::GameObject*> enemies = EnemyManager::GetInstance()->GetAllEnemies();
	bool hit = false;
	for (int i = 0; i < enemies.size(); i++)
	{
		//色初期化
		enemies[i]->SetColor(GE::Color::Red());
		if (GE::CollisionManager::CheckSphereToRay(enemies[i]->GetCollider(), enemies[i]->GetTransform()->position, rayPos, rayDir))
		{
			hit = true;
			lockOnEnemy.object = enemies[i];
		}
	}
	if (hit) { rayHitCount += 1.0 * GameTime; }
	else { rayHitCount = 0; }
	//指定した秒数カーソル合わせたら
	if (rayHitCount > rayHitSecond)
	{
		//ロックオンしている敵を青くする
		lockOnEnemy.object->SetColor(GE::Color::Blue());

		isLockOnStart = false;
	}
	else
	{
		isLockOnStart = true;
	}
}//EaseIn関係がよくわからなかったから一時的に追加
const float PlayerComponent::easeIn(const float start, const float end, float time)
{
	return start * (1.0f - time * time) + end * time * time;
}
