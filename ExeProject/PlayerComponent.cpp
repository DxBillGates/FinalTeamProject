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

float PlayerComponent::frameRate;
PlayerComponent::PlayerStatas PlayerComponent::statas;
GE::Math::Vector3 PlayerComponent::onTheTreePosition = { 0,250,200 };	//木の上で体の高さ調整用
int PlayerComponent::hitStopTime = 15;								// ヒットストップの長さ
float PlayerComponent::body_direction_LerpTime = 50.0f;				//ダッシュ後体の角度の遷移
float PlayerComponent::pushStartTime = 20.0f;						//キーを押してから操作できるようになるまでのカウント
float PlayerComponent::stayLandLerpTime = 150.0f;					//木に着陸するラープ長さ
GE::Math::Vector3 PlayerComponent::gravity = { 0,0.5,0 };			//重力
float PlayerComponent::rayHitSecond = 144.0f;						//ロックオンする照準を合わせる長さ
float PlayerComponent::normal_speed = 20.0f;						//通常時のスピード
float PlayerComponent::current_speed = normal_speed;				//現在のスピード
float PlayerComponent::damageSpeed = 0.0f;							//敵にヒットしたときにダメージが入るスピード
int PlayerComponent::colectMax = 10;								//一度に掴めるえさの数
float PlayerComponent::worldRadius = 38000.0f;						//端この壁までの長さ
float PlayerComponent::lockOnLength = 10000.0f;						//ロックオンできる距離
float PlayerComponent::moreTimesLockOnLength = 10000.0f;			//連続で二回目以降の敵をロックオンできる距離
int PlayerComponent::lockOnInterval = 300.0f;						//再度ロックオンできるまでのインターバル

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

	hitStopCount = hitStopTime;
	stayLandLerpEasingCount = stayLandLerpTime;
	//姿勢遷移
	body_direction_LerpCount = body_direction_LerpTime;
	//レティクルの位置
	center = GE::Window::GetWindowSize() / 2.0 + GE::Math::Vector2(0, -100);
	//レイキャストのスプライトを描画するか
	is_rayCast_active = false;

	CameraControl::GetInstance()->SetGraphicsDevice(graphicsDevice);
	CameraControl::GetInstance()->Initialize();
	InputManager::GetInstance()->Initialize();

	animator = GE::SkinMeshManager::GetInstance()->Get("Bird");
	animator.Initialize();
	animator.PlayAnimation(3, false);
	//収集物
	PlayerColectObject::GetInstance()->Start(colectMax, gameObject);
}
void PlayerComponent::Update(float deltaTime)
{
	frameRate = 1.0f / deltaTime;
	const float f = 144.0f / frameRate;

	crashParticle.Update(f);

	//testBGM->Start();
	InputManager::GetInstance()->Update();
	const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
	GE::Math::GetScreenToRay(center, &rayPos, &rayDir, cameraInfo.viewMatrix, cameraInfo.projMatrix, GE::Math::Matrix4x4::GetViewportMatrix(GE::Window::GetWindowSize()));

	//ヒットストップのカウント
	if (!InputManager::GetInstance()->GetLockonButton())
	{
		if (hitStopCount < hitStopTime)
		{
			GE::GameSetting::Time::SetGameTime(0.01);
			hitStopCount += 1;
		}
		else { GE::GameSetting::Time::SetGameTime(1.0); }
	}
	//操作
	Control(f);
	CameraControl::GetInstance()->SetTargetObject(gameObject);
	//収集物
	PlayerColectObject::GetInstance()->Update(f, colectCount);
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
		if (statas == PlayerStatas::STAY_TREE || statas == PlayerStatas::GO_TREE || statas == PlayerStatas::TITLE)
		{

		}
		else
		{
			if (InputManager::GetInstance()->GetActionButton())
			{
				stayLandLerpEasingCount = 0.0f;
				currentPosition = transform->position;
				statas = PlayerStatas::GO_TREE;
			}
		}
	}
}

void PlayerComponent::OnCollisionEnter(GE::GameObject* other)
{
	if (statas != PlayerStatas::GO_TREE && statas != PlayerStatas::STAY_TREE && statas != PlayerStatas::TITLE)
	{
		if (other->GetTag() == "ground" || other->GetTag() == "StartTree")
		{
			//各法線セット
			Reflection(gameObject->GetHitNormal());
			crashParticle.Fire(transform->position, gameObject->GetHitNormal(), other->GetColor());
			PlayerColectObject::GetInstance()->SetFallen(gameObject->GetHitNormal());
			return;
		}
		else if (other->GetTag() == "tile")
		{
			//各法線セット
			Reflection(GE::Math::Vector3(0, 1, 0));
			crashParticle.Fire(transform->position, GE::Math::Vector3(0, 1, 0), other->GetColor());
			PlayerColectObject::GetInstance()->SetFallen({ 0,1,0 });
			return;

		}
		else if (other->GetTag() == "fieldtree")
		{
			GE::Math::Vector3 dir = transform->position - other->GetTransform()->position;
			dir = dir.Normalize();
			GE::Math::Vector3 hitNotmal = GE::Math::Vector3(dir.x, 0, dir.z);
			Reflection(hitNotmal);
			crashParticle.Fire(transform->position, GE::Math::Vector3(0, 1, 0), other->GetColor());
			PlayerColectObject::GetInstance()->SetFallen(hitNotmal);

		}
	}
	//GE::Utility::Printf("PlayerComponent OnCollisionEnter\n");
	if (other->GetTag() == "enemy"|| other->GetTag() == "frog")
	{
		//一定上の速度を満たしていない
		if (!IsSpeedy())
		{

		}
		else
		{
			if (other == lockOnEnemy.object)
			{
				lockOnEnemy.object = nullptr;
			}
			audioManager->Use("catch2")->Start();
			//収集物 +1
			colectCount < colectMax ? colectCount++ : 0;
		}
		hitStopCount = 0;
		CameraControl::GetInstance()->ShakeStart({ 70,70 }, 30);
	}
}

void PlayerComponent::OnCollisionExit(GE::GameObject* other)
{
	GE::Utility::Printf("PlayerComponent OnCollisionExit\n");
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
	auto joycon = inputDevice->GetJoyconL();
	if (joycon == nullptr)return;
	GE::Math::Vector2 value = { (float)joycon->GetStick().x,(float)joycon->GetStick().y };
	ImGui::InputFloat2("joyStick", value.value);
	ImGui::Checkbox("IsDraw", &isDraw);
}
bool PlayerComponent::IsSpeedy()
{
	if (current_speed > damageSpeed)
	{
		return true;
	}
	return false;
}
void PlayerComponent::Control(float deltaTime)
{
	const float distance = abs(GE::Math::Vector3::Distance(transform->position, FieldObjectManager::GetInstance()->StartPosition));
	if (statas != PlayerStatas::CRASH)
	{
		if (worldRadius < distance)
		{
			Reflection(GE::Math::Vector3(transform->position - FieldObjectManager::StartPosition).Normalize());
		}
	}
	bool loop = false;
	switch (statas)
	{
	case PlayerComponent::PlayerStatas::TITLE:

		transform->position = FieldObjectManager::GetInstance()->StartPosition + onTheTreePosition;
		if (Title::GetInstance()->GetSelect(Title::States::start))
		{
			Title::GetInstance()->states = Title::States::serectNum;
			statas = PlayerStatas::STAY_TREE;
		}

		break;
	case PlayerComponent::PlayerStatas::DEBUG:
		break;
	case PlayerComponent::PlayerStatas::MOVE:
		transform->position += transform->GetForward() * current_speed * deltaTime * GE::GameSetting::Time::GetGameTime() - gravity;
		//Key押したらPlayerState::DASHに変わる
		if (InputManager::GetInstance()->GetActionButton()) { statas = PlayerStatas::DASH; }
		if (lockOnIntervalCount >= lockOnInterval)
		{
			if (InputManager::GetInstance()->GetLockonButton())
			{
				isLockOnStart = true;
				//最も近くて前方にいる敵をセット
				SearchNearEnemy();
			}
			else
			{
				isLockOnStart = false;
			}
		}
		else
		{
			lockOnIntervalCount += deltaTime;
			isLockOnStart = false;
		}
		//RayCast(deltaTime);
		//ロックオンして攻撃
		LockOn();
		break;
	case PlayerComponent::PlayerStatas::DASH:
		Dash(100.0f, 80.0f, deltaTime, transform->GetForward());
		break;
	case PlayerComponent::PlayerStatas::CRASH:
		//ダッシュから切り替わった時用初期化
		current_speed = normal_speed;
		//移動
		transform->position += transform->GetForward() * current_speed * deltaTime * GE::GameSetting::Time::GetGameTime();

		if (InputManager::GetInstance()->GetActionButton())
		{
			//体のオイラー角を取得してセット
			body_direction = transform->rotation.EulerRadian();
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
				lockOnEnemy.direction = GE::Math::Vector3(lockOnEnemy.object->GetTransform()->position - transform->position).Normalize();
				loop = true;
			}
			else {
				isLockOn = false;
			}
		}
		Dash(100.f, 20.f, deltaTime, lockOnEnemy.direction, loop);
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
				startCouunt+=deltaTime;
				//MoveFromStop
				animator.PlayAnimation(2, false);
				audioManager->Use("flapping1")->Start();
			}
		}
		else if (startCouunt < 2.0f) { startCouunt++; }//1フレーム更新しないとSkinMeshAnimator::IsEndがTrueのままになるからいったんこれで…
		else
		{
			if (animator.IsEndAnimation())
			{
				statas = PlayerStatas::MOVE;
				//Flapping
				animator.PlayAnimation(1, false);
			}
			else
			{
				//仮飛び降り
				transform->position += transform->GetForward() * 20.0f * deltaTime * GE::GameSetting::Time::GetGameTime() - gravity * 5.0f;
			}
		}
		break;
	default:
		break;
	}
	if (statas != PlayerStatas::CRASH)
	{
		GE::Math::Quaternion BODY_DIRECTION =
			GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), body_direction.y)
			* GE::Math::Quaternion(GE::Math::Vector3(0, 0, 1), body_direction.z)
			* GE::Math::Quaternion(GE::Math::Vector3(1, 0, 0), body_direction.x);
		//ダッシュ後体の角度の遷移
		if (body_direction_LerpCount < body_direction_LerpTime)
		{
			body_direction_LerpCount += 1 * GE::GameSetting::Time::GetGameTime();
			transform->rotation = GE::Math::Quaternion::Lerp(body_direction_LockOn, BODY_DIRECTION, body_direction_LerpCount / body_direction_LerpTime);
		}
		else { transform->rotation = BODY_DIRECTION; }

		if (statas != PlayerStatas::TITLE)
		{
			//キーボードで移動操作
			KeyboardMoveControl(deltaTime);
		}
	}
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
	//// ジョイコン操作中の際の姿勢制御
	//GE::Joycon* joycon = inputDevice->GetJoyconL();
	//if (joycon == nullptr)return;
	//GE::Vector3Int16 gyroData = joycon->GetGyroscope();
	//gyro = { (float)gyroData.y,(float)-gyroData.z,(float)-gyroData.x };

	//// コントローラーから姿勢を更新し続ける
	//quat *= GE::Math::Quaternion(gyro.Normalize(), GE::Math::ConvertToRadian(gyro.Length() * 1.f / 144.f));

	//if (InputManager::GetInstance()->GetCurrentInputDeviceState() != InputManager::InputDeviceState::JOYCON)return;
	//const float GYRO_OFFSET = 0.05f;
	//GE::Math::Vector3 quatVector = { quat.x,quat.y,quat.z, };
	// 
	//body_direction.x += quatVector.x / 20.f;
	//body_direction.y += quatVector.y / 20.f;
	//body_direction.z += quatVector.z / 20.f;

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
		body_direction_LerpCount = 0;
	}
	else
	{
		current_speed = easeIn(dash_speed, normal_speed, dashEasingCount / dash_time);
	}
	if (dashEasingCount < dash_time) { dashEasingCount += 1 * GE::GameSetting::Time::GetGameTime(); }
	else { statas = PlayerStatas::MOVE; dashEasingCount = 0.0f; body_direction_LerpCount = 0; animator.PlayAnimation(1, false); }


	body_direction_LockOn = GE::Math::Quaternion::LookDirection(direction);
	transform->rotation = body_direction_LockOn;


	transform->position += transform->GetForward() * current_speed * deltaTime * GE::GameSetting::Time::GetGameTime();
}

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
	if (hit) { rayHitCount += 1.0 * GE::GameSetting::Time::GetGameTime(); }
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
}

void PlayerComponent::Reflection(GE::Math::Vector3 normal)
{
	statas = PlayerStatas::CRASH;
	audioManager->Use("hitWall")->Start();
	transform->rotation = GE::Math::Quaternion::LookDirection(GE::Math::Vector3::Reflection(transform->GetForward(), normal, 2.0f));
	CameraControl::GetInstance()->ShakeStart({ 70,70 }, 30);
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
