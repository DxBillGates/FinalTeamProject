#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GUI\GUIManager.h        >
#include <GatesEngine/Header/GameFramework/Collision/CollisionManager.h>
#include <GatesEngine/Header\GameFramework/GameSetting.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

#include"PlayerComponent.h"
#include"EnemyManager.h"
#include "InputManager.h"
#include "CameraControl.h"
#include "FieldObjectManager.h"
#include "Title.h"
#include "TimeLimit.h"
#include "PlayerColectObject.h"
#include "ScreenUI.h"
#include "Ranking.h"

float PlayerComponent::frameRate;
PlayerComponent::PlayerStatas PlayerComponent::statas;
PlayerComponent::LockOnState PlayerComponent::lockonState;
GE::Math::Vector3 PlayerComponent::onTheTreePosition = { 0,250,200 };	//木の上で体の高さ調整用
int PlayerComponent::hitStopTime = 50;									// ヒットストップの長さ
float PlayerComponent::pushStartTime = 20.0f;							//キーを押してから操作できるようになるまでのカウント
float PlayerComponent::stayLandLerpTime = 150.0f;						//木に着陸するラープ長さ
GE::Math::Vector3 PlayerComponent::gravity = { 0,0.5,0 };				//重力
float PlayerComponent::normal_speed = 20.0f;							//通常時のスピード
float PlayerComponent::current_speed = normal_speed;					//現在のスピード
int PlayerComponent::colectMax = 10;									//一度に掴めるえさの数
float PlayerComponent::worldRadius = 76000.0f;							//端この壁までの長さ
float PlayerComponent::lockOnLength = 10000.0f;							//ロックオンできる距離
float PlayerComponent::moreTimesLockOnLength = 10000.0f;				//連続で二回目以降の敵をロックオンできる距離
int PlayerComponent::lockOnInterval = 250.0f;							//再度ロックオンできるまでのインターバル
float PlayerComponent::comboInterval = 70.0f;
bool PlayerComponent::isGoTree = false;
bool PlayerComponent::dashMode = false;
bool PlayerComponent::isJoyconUsing = false;
int PlayerComponent::combo = 0;
int PlayerComponent::takeEnemyCount = 0;

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
	//激突時パーティクル
	crashParticle.Start();

	isDraw = true;
	GE::Utility::Printf("PlayerComponent Start()\n");
	inputDevice = GE::InputDevice::GetInstance();
	transform->position = FieldObjectManager::GetInstance()->StartPosition + onTheTreePosition;
	transform->scale = { 10,10,10 };

	statas = PlayerStatas::TITLE;

	body_direction = { 0,-1.57f,0 };
	dashEasingCount = 0.0;
	startCouunt = 0.0f;
	colectCount = 0;
	//
	lockOnIntervalCount = lockOnInterval;

	statasChangeCount = 0;

	hitStopCount = (float)hitStopTime;
	stayLandLerpEasingCount = stayLandLerpTime;
	//レティクルの位置
	center = GE::Window::GetWindowSize() / 2.0 + GE::Math::Vector2(0, -100);

	CameraControl::GetInstance()->SetGraphicsDevice(graphicsDevice);
	CameraControl::GetInstance()->Initialize();
	InputManager::GetInstance()->Initialize();

	animator = GE::SkinMeshManager::GetInstance()->Get("Bird");
	animator.Initialize();
	animator.PlayAnimation(3, false);
	//収集物
	PlayerColectObject::GetInstance()->Start(colectMax, gameObject);

	comboCount = comboInterval;
	combo = 0;
	takeEnemyCount = 0;
	lockonState = LockOnState::NONE;

	if (dashMode)
	{
		hitStopTime = 20;
	}
	else
	{
		hitStopTime = 50;
	}
}
void PlayerComponent::Update(float deltaTime)
{
	preQuat = quat;

	frameRate = 1.0f / deltaTime;
	const float f = 144.0f / frameRate;
	isGoTree = false;

	crashParticle.Update(f);

	InputManager::GetInstance()->Update(deltaTime);
	const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
	GE::Math::GetScreenToRay(center, &rayPos, &rayDir, cameraInfo.viewMatrix, cameraInfo.projMatrix, GE::Math::Matrix4x4::GetViewportMatrix(GE::Window::GetWindowSize()));

	//ヒットストップのカウント
	if (!InputManager::GetInstance()->GetLockonButton())
	{
		if (hitStopCount < hitStopTime)
		{
			GE::GameSetting::Time::SetGameTime(0.01);
			hitStopCount += f;
		}
		else { GE::GameSetting::Time::SetGameTime(1.0); }
	}
	if (comboCount < comboInterval)
	{
		comboCount += 1.0f * f * GE::GameSetting::Time::GetGameTime();
	}
	else
	{
		combo = 0;
	}
	//ゲームモード別の速さ設定
	if (dashMode)
	{
		normal_speed = 100.0f;
	}
	else
	{
		normal_speed = 20.0f;
	}
	current_speed = normal_speed;

	//操作
	Control(f);
	CameraControl::GetInstance()->SetTargetObject(gameObject);
	//収集物
	PlayerColectObject::GetInstance()->Update(f);
	//カメラコントロールの更新
	CameraControl::GetInstance()->Update(f);

	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::LCONTROL) || inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::RCONTROL))
	{
		//D0押したら移動停止＊デバッグ用
		if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D0))
		{
			if (statas != PlayerStatas::DEBUG)
			{
				statas = PlayerStatas::DEBUG;
			}
			else
			{
				if (isDraw)
				{
					isDraw = false;
				}
				else
				{
					isDraw = true;
					statas = PlayerStatas::MOVE;
				}
			}
		}
		//めり込んだらD9キーで木に強制送還
		if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D9))
		{
			stayLandLerpEasingCount = 0.0f;
			currentPosition = transform->position;
			statas = PlayerStatas::GO_TREE;
		}
	}
	//床貫通したら強制送還
	float tileHigher = FieldObjectManager::GetInstance()->GetTile()->GetTransform()->position.y - 100;
	if (transform->position.y < tileHigher)
	{
		stayLandLerpEasingCount = 0.0f;
		currentPosition = transform->position;
		statas = PlayerStatas::GO_TREE;
	}

	animator.Update(deltaTime);
}

void PlayerComponent::DrawShadow()
{
	if (!isDraw)return;

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSkinMeshShader");

	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();
	GE::Material material;
	material.color = GE::Color::White();

	animator.SetAnimationData(graphicsDevice, modelMatrix);
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });

	graphicsDevice->DrawMesh("Player");

	//収集物
	PlayerColectObject::GetInstance()->DrawShadow(graphicsDevice);
}

void PlayerComponent::Draw()
{
	crashParticle.Draw(graphicsDevice);

	if (!isDraw)return;

	//graphicsDevice->SetLayer("effectLayer");
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSkinMeshShader");
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	GE::Material material;
	material.color = GE::Color::White();
	animator.SetAnimationData(graphicsDevice, modelMatrix);

	//renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	GE::CameraInfo cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
	cameraInfo.lightMatrix = gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->GetVPMatrix();
	gameObject->GetGameObjectManager()->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->SetDirectionalLightInfo();
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });


	graphicsDevice->DrawMesh("Player");
	//収集物
	PlayerColectObject::GetInstance()->Draw(graphicsDevice, gameObject->GetGameObjectManager());
}

void PlayerComponent::LateDraw()
{

}

void PlayerComponent::OnCollision(GE::GameObject* other)
{
	//GE::Utility::Printf("PlayerComponent OnCollision(GameObject* other) : hit\n");

	if (other->GetTag() == "nest")
	{
		if (statas == PlayerStatas::STAY_TREE || statas == PlayerStatas::GO_TREE || statas == PlayerStatas::TITLE || statas == PlayerStatas::TITLE_MENU)
		{
		}
		else
		{
			isGoTree = true;
			if (colectCount > 0)
			{
				stayLandLerpEasingCount = 0.0f;
				dashEasingCount = 0.0f;
				current_speed = normal_speed;
				currentPosition = transform->position;
				statas = PlayerStatas::GO_TREE;
			}
		}
	}
}

void PlayerComponent::OnCollisionEnter(GE::GameObject* other)
{
	if (statas != PlayerStatas::GO_TREE && statas != PlayerStatas::STAY_TREE && statas != PlayerStatas::TITLE && statas != PlayerStatas::TITLE_MENU)
	{
		if (dashMode)
		{
			if (other->GetTag() == "ground" || other->GetTag() == "StartTree" || other->GetTag() == "tile" || other->GetTag() == "fieldtree")
			{
				//各法線セット
				Reflection(gameObject->GetHitNormal());
				crashParticle.Fire(transform->position, gameObject->GetHitNormal(), other->GetColor());
				statas = PlayerStatas::OVER;
				dashMode = false;
				//スコアセット
				Ranking::GetInstance()->SetRanking(takeEnemyCount);
				return;

			}
		}
		else
		{
			if (other->GetTag() == "ground" || other->GetTag() == "StartTree")
			{
				if (GE::Math::Vector3::Dot(transform->GetForward(), gameObject->GetHitNormal()) < 0.0)
				{
					//各法線セット
					Reflection(gameObject->GetHitNormal());
					crashParticle.Fire(transform->position, gameObject->GetHitNormal(), other->GetColor());
					return;
				}
			}
			else if (other->GetTag() == "tile")
			{
				if (statas != PlayerStatas::LOCKON_SHOOT)
				{
					//各法線セット
					Reflection(GE::Math::Vector3(0, 1, 0));
					crashParticle.Fire(transform->position, GE::Math::Vector3(0, 1, 0), other->GetColor());
					return;
				}
			}
			else if (other->GetTag() == "fieldtree")
			{
				GE::Math::Vector3 dir = transform->position - other->GetTransform()->position;
				dir = dir.Normalize();
				GE::Math::Vector3 hitNormal = GE::Math::Vector3(dir.x, 0, dir.z);
				Reflection(hitNormal);
				crashParticle.Fire(transform->position, GE::Math::Vector3(0, 1, 0), other->GetColor());
			}
		}
	}

	//GE::Utility::Printf("PlayerComponent OnCollisionEnter\n");
	if (other->GetTag() == "enemy" || other->GetTag() == "frog")
	{
		if (other->GetComponent<Enemy>()->statas == Enemy::Statas::ALIVE)
		{
			if (!dashMode)
			{

				if (other == lockOnEnemy.object)
				{
					lockOnEnemy.object = nullptr;
					if (lockOnDashDirection.y < 0)
						lockOnDashDirection = { transform->GetForward().x,-transform->GetForward().y ,transform->GetForward().z };
				}
				//収集物 +1
				colectCount < colectMax ? colectCount++ : 0;
				PlayerColectObject::GetInstance()->AddObject("model" + other->GetComponent<Enemy>()->modelName);
			}
			else
			{
				//コンボ加算と初期化
				comboCount = 0.0f;
				combo++;
				takeEnemyCount++;
				//パーティクル
				crashParticle.Fire(transform->position, -transform->GetForward(), other->GetColor());
				if (TimeLimit::GetInstance()->GetTenSeconds() < 3)
				{
					TimeLimit::GetInstance()->AddSeconds(combo);
				}
				//コンボUIシェイク！
				ScreenUIManager::GetInstance()->viveVelocity = { 80,80 };
			}
			if (combo > 3) { audioManager->Use("jag")->Start(); }
			else { audioManager->Use("catch2")->Start(); }

			hitStopCount = 0.0f;
			CameraControl::GetInstance()->ShakeStart({ 30,30 }, 30);
		}
	}
}

void PlayerComponent::OnCollisionExit(GE::GameObject* other)
{

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
	ImGui::DragFloat3("TotalGyroVector", totalGyro.value, dragSpeed, -1, 1);
	ImGui::InputFloat4("quat", quat.value);
	ImGui::InputFloat3("accelerometer", accelerometer.value);
	ImGui::InputFloat3("quatAngle", quat.EulerAngle().value);

	GE::Math::Vector3 inputAxis = InputManager::GetInstance()->GetAxis();
	ImGui::InputFloat3("inputAxis", inputAxis.value);
	auto joycon = inputDevice->GetJoyconL();
	if (joycon == nullptr)return;
	GE::Math::Vector2 value = { (float)joycon->GetStick().x,(float)joycon->GetStick().y };
	ImGui::InputFloat2("joyStick", value.value);
	ImGui::Checkbox("IsDraw", &isDraw);
	ImGui::Text("%.3f", accelerometer.Length());
	ImGui::Text("%.3f", joycon->GetDefaultAccelerometerDiff());
	ImGui::Text("%.3f", joycon->GetGyroscope().Length());
}
void PlayerComponent::Control(float deltaTime)
{
	const float distance = abs(GE::Math::Vector3::Distance(transform->position, FieldObjectManager::GetInstance()->StartPosition));

	GE::Math::Quaternion BODY_DIRECTION =
		GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), body_direction.y)
		* GE::Math::Quaternion(GE::Math::Vector3(0, 0, 1), body_direction.z)
		* GE::Math::Quaternion(GE::Math::Vector3(1, 0, 0), body_direction.x);

	if (worldRadius < distance)
	{
		Reflection(-GE::Math::Vector3(transform->position - FieldObjectManager::StartPosition).Normalize(), true);
	}
	bool loop = false;
	switch (statas)
	{
	case PlayerComponent::PlayerStatas::TITLE:

		transform->position = FieldObjectManager::GetInstance()->StartPosition + onTheTreePosition;
		if (InputManager::GetInstance()->GetActionButton())
		{
			statas = PlayerStatas::TITLE_MENU;
			Title::GetInstance()->isActive = true;
		}

		break;
	case PlayerComponent::PlayerStatas::TITLE_MENU:
		if (!dashMode)
		{
			if (Title::GetInstance()->GetSelect(Title::States::start))
			{
				statas = PlayerStatas::STAY_TREE;
				Title::GetInstance()->states = Title::States::serectNum;
			}
		}
		else
		{
			statas = PlayerStatas::STAY_TREE;
		}

		break;
	case PlayerComponent::PlayerStatas::DEBUG:
		break;
	case PlayerComponent::PlayerStatas::MOVE:
		transform->position += transform->GetForward() * current_speed * deltaTime * GE::GameSetting::Time::GetGameTime() - gravity;
		if (!dashMode)
		{
			//Key押したらPlayerState::DASHに変わる
			if (InputManager::GetInstance()->GetActionButton()) { statas = PlayerStatas::DASH; }
			if (lockOnIntervalCount >= lockOnInterval)
			{
				if (InputManager::GetInstance()->GetLockonButton())
				{
					isLockOnStart = true;
					lockonState = LockOnState::SEARCH;
					//最も近くて前方にいる敵をセット
					SearchNearEnemy();
				}
				else
				{
					lockonState = LockOnState::NONE;
					isLockOnStart = false;
				}
			}
			else
			{
				lockOnIntervalCount += deltaTime;
				isLockOnStart = false;
			}

			//ロックオンして攻撃
			LockOn();
		}
		break;
	case PlayerComponent::PlayerStatas::DASH:
		NormalDash(100.0f, 90.0f, deltaTime);
		break;
	case PlayerComponent::PlayerStatas::CRASH:
		//ダッシュから切り替わった時用初期化
		current_speed = normal_speed;
		//移動
		transform->position += transform->GetForward() * current_speed * deltaTime * GE::GameSetting::Time::GetGameTime();
		//body_direction.z += 0.03f * deltaTime * GE::GameSetting::Time::GetGameTime();

		if (InputManager::GetInstance()->GetActionButton())
		{
			//体のオイラー角を取得してセット
			animator.PlayAnimation(0, false);
			statas = PlayerStatas::MOVE;
			audioManager->Use("flapping1")->Start();
		}
		break;
	case PlayerComponent::PlayerStatas::LOCKON_SHOOT:
		//ロックオンのインターバルのカウントを初期化
		lockOnIntervalCount = 0;
		if (lockOnEnemy.object != nullptr)
		{
			if (lockOnEnemy.object->GetComponent<Enemy>()->statas != Enemy::Statas::DEAD)
			{
				lockOnEnemy.direction = GE::Math::Vector3(lockOnEnemy.object->GetTransform()->position + GE::Math::Vector3(0, 100, 0) - transform->position).Normalize();
				loop = true;
			}
			else {
				isLockOn = false;
			}
			lockOnDashDirection = lockOnEnemy.direction;
		}
		Dash(100.f, 20.f, deltaTime, lockOnDashDirection, loop);
		lockonState = LockOnState::NONE;
		break;
	case PlayerComponent::PlayerStatas::GO_TREE:
		if (stayLandLerpEasingCount < stayLandLerpTime)
		{
			stayLandLerpEasingCount += deltaTime * GE::GameSetting::Time::GetGameTime();
			transform->position = GE::Math::Vector3::Lerp(currentPosition, FieldObjectManager::GetInstance()->StartPosition + onTheTreePosition, stayLandLerpEasingCount / stayLandLerpTime);
		}
		else
		{
			startCouunt = 0.0f;
			//stayアニメーション
			animator.PlayAnimation(3, false);
			//収集物お持ち帰り
			StartTree::collectCount += colectCount;
			TimeLimit::GetInstance()->AddSeconds(10 * colectCount);
			colectCount = 0;
			PlayerColectObject::GetInstance()->ClearObject();
			//着陸
			statas = PlayerStatas::STAY_TREE;
		}
		break;
	case PlayerComponent::PlayerStatas::STAY_TREE:
		if (startCouunt == 0.0f)
		{
			transform->position = FieldObjectManager::GetInstance()->StartPosition + onTheTreePosition;
			if (InputManager::GetInstance()->GetActionButton())
			{
				TimeLimit::GetInstance()->isActive = true;
				startCouunt += deltaTime;
				//MoveFromStop
				animator.PlayAnimation(0, false);
				audioManager->Use("flapping1")->Start();
			}
		}
		else
		{
			if (animator.IsEndAnimation())
			{
				statas = PlayerStatas::MOVE;
			}
			else
			{
				//仮飛び降り
				transform->position += transform->GetForward() * 20.0f * deltaTime * GE::GameSetting::Time::GetGameTime() - gravity * 5.0f;
			}
		}
		break;
	case PlayerComponent::PlayerStatas::OVER:

		break;
	}
	if (statas != PlayerStatas::TITLE && statas != PlayerStatas::CRASH)
	{
		//キーボードで移動操作
		KeyboardMoveControl(deltaTime);
	}
	transform->rotation = BODY_DIRECTION;
}
void PlayerComponent::KeyboardMoveControl(float deltaTime)
{
	GE::Math::Vector3 inputAxis = InputManager::GetInstance()->GetAxis(0, InputManager::InputCtrlAxisState::STICK);

	GE::Math::Vector3 rota = GE::Math::Vector3(0.01 * inputAxis.x, 0.005 * inputAxis.y, 0.006 * inputAxis.x) * deltaTime * GE::GameSetting::Time::GetGameTime();
	if (inputAxis.x != 0)
	{
		body_direction.y += rota.x;
		body_direction.z -= rota.z;

		body_direction.z = abs(body_direction.z) > 0.3f ? 0.3f * ((body_direction.z > 0) ? 1 : -1) : body_direction.z;
	}
	else if (statas != PlayerStatas::DEBUG)
	{
		abs(body_direction.z) < 0.005 ? body_direction.z = 0 : body_direction.z > 0.0 ? body_direction.z -= 0.005 * GE::GameSetting::Time::GetGameTime() : body_direction.z += 0.005 * GE::GameSetting::Time::GetGameTime();
	}

	if (inputAxis.y != 0)
	{
		body_direction.x -= rota.y;
		body_direction.x = abs(body_direction.x) > 1.57f ? 1.57f * ((body_direction.x > 0) ? 1 : -1) : body_direction.x;
	}
	else if (statas != PlayerStatas::DEBUG)
	{
		abs(body_direction.x) < 0.01 ? body_direction.x = 0 : body_direction.x > 0.0 ? body_direction.x -= 0.01 * GE::GameSetting::Time::GetGameTime() : body_direction.x += 0.01 * GE::GameSetting::Time::GetGameTime();
	}
	// ジョイコン操作中の際の姿勢制御
	GE::Joycon* joycon = inputDevice->GetJoyconL();
	if (joycon == nullptr)return;

	GE::Math::Vector3 gyroData = joycon->GetSensorFusion();
	gyro = { (float)gyroData.y,(float)-gyroData.z,(float)-gyroData.x };

	// コントローラーから姿勢を更新し続ける
	quat *= GE::Math::Quaternion(gyro.Normalize(), GE::Math::ConvertToRadian(gyro.Length() * 1.f / 144.f));

	float joyconAccDiff = joycon->GetDefaultAccelerometerDiff();
	accelerometer = joycon->GetAccelerometer();

	GE::Math::Vector3  preQuatAngle, quatAngle;
	preQuatAngle = preQuat.EulerAngle();
	quatAngle = quat.EulerAngle();

	const float MAX_ANGLE = 5;

	// ジョイコンが水平に持たれていてかつ、gyroから作成しているクォータニオンの姿勢が{0,0,0,1}に近づいた際に姿勢をもとに戻す
	if (joyconAccDiff > 0.95f)
	{
		quat.z = 0;

		// x軸回転を補正
		if (std::fabsf(preQuatAngle.x) > MAX_ANGLE)
		{
			if (quatAngle.x < MAX_ANGLE && quatAngle.x > -MAX_ANGLE)
			{
				quat.x = 0;
			}
		}
		// y軸回転補正
		if (std::fabsf(preQuatAngle.y) > MAX_ANGLE * 2)
		{
			if (quatAngle.y < MAX_ANGLE * 2 && quatAngle.y > -MAX_ANGLE * 2)
			{
				quat.y = 0;
			}
		}
	}

	if (joycon->GetTriggerButton(GE::JoyconButtonData::MINUS))
	{
		quat = { 0,0,0,1 };
	}

	isJoyconUsing = false;
	if (InputManager::GetInstance()->GetCurrentInputDeviceState() != InputManager::InputDeviceState::JOYCON)return;
	isJoyconUsing = true;
	const float GYRO_OFFSET = 0.05f;
	GE::Math::Vector3 quatVector = { quat.x,quat.y,quat.z, };

	body_direction.x += quatVector.x / 10.f;
	body_direction.y += quatVector.y / 10.f;
	body_direction.z += quatVector.z / 10.f;

	GE::Math::Vector3 bodyDirectionMax;
	bodyDirectionMax = { 1.0f,100000,0.75f };
	body_direction = GE::Math::Vector3::Min(-bodyDirectionMax, GE::Math::Vector3::Max(bodyDirectionMax, body_direction));
}

void PlayerComponent::SearchNearEnemy(bool isForward)
{
	std::vector<GE::GameObject*> enemies = EnemyManager::GetInstance()->GetAllEnemies();
	float result = 100000;
	int a = 0;
	bool look = false;
	float whichLockOnLength;
	//サーチする距離をセット
	if (!isForward) { whichLockOnLength = lockOnLength; }
	else { whichLockOnLength = moreTimesLockOnLength; }

	for (int i = 0; i < enemies.size(); i++)
	{
		float distance = abs(GE::Math::Vector3::Distance(transform->position, enemies[i]->GetTransform()->position));
		GE::Math::Vector3 enemyDirection = enemies[i]->GetTransform()->position - transform->position;
		//色初期化
		enemies[i]->SetColor(GE::Color::Red());
		//生きているか＆前側にいる中で最も近い敵&&LockOnLengthより近い距離か
		if (enemies[i]->GetComponent<Enemy>()->statas != Enemy::Statas::DEAD
			&& distance < whichLockOnLength)
		{
			//引数をTrueにすると前にいるか関係なくロックオンする()
			if (GE::Math::Vector3::Dot(transform->GetForward(), enemyDirection.Normalize()) > 0.8 || isForward)
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
		GE::GameSetting::Time::SetGameTime(0.2);
		//最も近い敵
		lockOnEnemy.object = enemies[a];
		//ロックオンしている敵を青くする
		lockOnEnemy.object->SetColor(GE::Color::Blue());
		lockonState = LockOnState::LOCKON_SLOW;
	}
}
void PlayerComponent::LockOn()
{
	if (lockOnEnemy.object != nullptr
		&& lockOnEnemy.object->GetComponent<Enemy>()->statas != Enemy::Statas::DEAD)
	{
		//Key押したらLockOn準備
		if (isLockOnStart) { isLockOn = true; }
		else { statas = PlayerStatas::LOCKON_SHOOT; }
	}
}

void PlayerComponent::Dash(float dash_speed, float dash_time, float deltaTime, GE::Math::Vector3 direction, bool loop)
{
	//FlyAnimation
	if (dashEasingCount == 0.0f) {
		animator.PlayAnimation(0, false);
		audioManager->Use("flapping1")->Start();
		audioManager->Use("air1")->Start();
	}
	if (loop)
	{
		current_speed = dash_speed;
		dashEasingCount = 0.0f;
	}
	else
	{
		current_speed = easeIn(dash_speed, normal_speed, dashEasingCount / dash_time);
	}
	if (dashEasingCount < dash_time) { dashEasingCount += 1 * GE::GameSetting::Time::GetGameTime(); }
	else {
		statas = PlayerStatas::MOVE; dashEasingCount = 0.0f; animator.PlayAnimation(1, false); ;
	}

	body_direction = GE::Math::Quaternion::LookDirection(direction).EulerRadian();

	transform->position += transform->GetForward() * current_speed * deltaTime * GE::GameSetting::Time::GetGameTime();
}

void PlayerComponent::NormalDash(float dash_speed, float dash_time, float deltaTime)
{
	//FlyAnimation
	if (dashEasingCount == 0.0f) {
		animator.PlayAnimation(0, false);
		audioManager->Use("flapping1")->Start();
		audioManager->Use("air1")->Start();
	}
	current_speed = easeIn(dash_speed, normal_speed, dashEasingCount / dash_time);

	if (dashEasingCount < dash_time) { dashEasingCount += 1 * GE::GameSetting::Time::GetGameTime(); }
	else {
		statas = PlayerStatas::MOVE; dashEasingCount = 0.0f; animator.PlayAnimation(1, false);
	}

	transform->position += transform->GetForward() * current_speed * deltaTime * GE::GameSetting::Time::GetGameTime();
}
void PlayerComponent::Reflection(GE::Math::Vector3 normal, bool reflection)
{
	auto a = transform->GetForward();
	statas = PlayerStatas::CRASH;
	audioManager->Use("hitWall")->Start();
	if (!reflection)
	{
		body_direction = GE::Math::Quaternion::LookDirection(GE::Math::Vector3::Reflection(transform->GetForward(), normal, 2.0f)).EulerRadian();
	}
	else
	{
		body_direction = GE::Math::Quaternion::LookDirection(normal).EulerRadian();
	}
	a = GE::Math::Vector3::Reflection(transform->GetForward(), normal, 2.0f);

	CameraControl::GetInstance()->ShakeStart({ 50,50 }, 30);
	//ロックオン中ならロックオンをキャンセル
	isLockOn = false; dashEasingCount = 0.0f; lockOnEnemy.object = nullptr;

	//収集物落下
	PlayerColectObject::GetInstance()->SetFallen(normal);
	colectCount > 0 ? colectCount-- : 1;
}
//EaseIn関係がよくわからなかったから一時的に追加
const float PlayerComponent::easeIn(const float start, const float end, float time)
{
	return start * (1.0f - time * time) + end * time * time;
}

GE::Math::Vector3 PlayerComponent::GetDirection()
{
	return body_direction;
}

void PlayerComponent::SetAudioManager(GE::AudioManager* a)
{
	audioManager = a;
}
