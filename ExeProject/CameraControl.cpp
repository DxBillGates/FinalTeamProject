#include "CameraControl.h"
#include "PlayerComponent.h"
#include "Clear.h"
#include "Over.h"
#include <GatesEngine/Header\GameFramework/GameSetting.h>
#include <GatesEngine/Header/Util/Random.h           >
#include"UIObject.h"
CameraControl* CameraControl::GetInstance()
{
	static CameraControl instance;
	return &instance;
}

void CameraControl::Create()
{

}

void CameraControl::Initialize()
{
	normal_cameraDistance = 300;
	dash_cameraDistance = 500;
	current_cameraDistance = normal_cameraDistance;

	shakeFlame = 0;
	count = 0.0f;

	range = {};
	cameraShake = {};

	graphicsDevice->GetMainCamera()->SetPosition({ 37750,600,15000 });
}

void CameraControl::Update(float deltaTime)
{
	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::DEBUG)return;
	float LERP_VALUE = 0.01f * GE::GameSetting::Time::GetGameTime() * deltaTime;
	UIObject::GetInstance()->cameraPosition = position;

	if (PlayerComponent::dashMode)
	{
		LERP_VALUE = 0.05f * GE::GameSetting::Time::GetGameTime() * deltaTime;
	}
	else
	{
		LERP_VALUE = 0.01f * GE::GameSetting::Time::GetGameTime() * deltaTime;
	}

	//体の角度計算
	if (abs(targetObject->GetTransform()->GetForward().y) < 0.6)
	{
		dir = atan2f(targetObject->GetTransform()->GetForward().x, targetObject->GetTransform()->GetForward().z);
	}
	//カメラの向き
	GE::Math::Vector3 direction = GE::Math::Vector3(target - position).Normalize();

	//カメラの更新
	GE::Math::Vector3 newCameraPosition;
	if (Clear::nowClear)
	{
		target = targetObject->GetTransform()->position;
		current_cameraDistance = normal_cameraDistance;
		newCameraPosition = { 2000,9000,-6000 };
	}
	else if (Over::nowOver)
	{
		//巣を映し終わったら
		if (position.y < 9000)
		{
			//加速最大ですか
			if (Over::CameraSpeed < 0.3f)
			{
				//だんだん加速
				Over::CameraSpeed += 0.001f;
			}
		}
		LERP_VALUE = Over::CameraSpeed * GE::GameSetting::Time::GetGameTime() * deltaTime;

		target = targetObject->GetTransform()->position;
		current_cameraDistance = normal_cameraDistance;
		newCameraPosition = { -1800,500,-9107 };
	}
	else if (PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE || PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE_MENU)
	{
		current_cameraDistance = 3000;
		GE::Math::Vector3 wind = GE::Math::Vector3(sinf(count), cosf(count), cosf(-count)) * 30;
		count += 0.01f * deltaTime;

		//注視点
		target = targetObject->GetTransform()->position + targetObject->GetTransform()->GetRight() * 2000 + wind;

		newCameraPosition = target
			- GE::Math::Vector3(targetObject->GetTransform()->GetForward().x * current_cameraDistance,
				-300,
				targetObject->GetTransform()->GetForward().z * current_cameraDistance);


		direction = GE::Math::Vector3(target - position).Normalize();
	}
	else if (PlayerComponent::statas == PlayerComponent::PlayerStatas::STAY_TREE)
	{
		current_cameraDistance = 2000;
		target = GE::Math::Vector3::Lerp(target, targetObject->GetTransform()->position, LERP_VALUE);
		newCameraPosition = target
			- GE::Math::Vector3(targetObject->GetTransform()->GetForward().x * current_cameraDistance,
				-300,
				targetObject->GetTransform()->GetForward().z * current_cameraDistance);
	}
	else
	{
		//target = GE::Math::Vector3::Lerp(target, targetObject->GetTransform()->position, 0.9f);
		target = targetObject->GetTransform()->position;
		current_cameraDistance = normal_cameraDistance;
		newCameraPosition = target - targetObject->GetTransform()->GetForward() * current_cameraDistance;
	}
	auto camera = dynamic_cast<GE::Camera3DDebug*>(graphicsDevice->GetMainCamera());
	//現在のポジション
	GE::Math::Vector3 beforeCameraPosition = {
		camera->GetCameraInfo().cameraPos.x,
		camera->GetCameraInfo().cameraPos.y,
		camera->GetCameraInfo().cameraPos.z,
	};

	//ダッシュ時
	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::DASH
		|| PlayerComponent::statas == PlayerComponent::PlayerStatas::LOCKON_SHOOT)
	{
		LERP_VALUE = 0.035f * GE::GameSetting::Time::GetGameTime();
	}
	position = GE::Math::Vector3::Lerp(beforeCameraPosition, newCameraPosition, LERP_VALUE) + cameraShake * GE::GameSetting::Time::GetGameTime();
	target += cameraShake * GE::GameSetting::Time::GetGameTime() * deltaTime;

	//カメラシェイク
	Shake(deltaTime);

	camera->SetDirection(direction);
	camera->SetPosition(position);
}

void CameraControl::Shake(float deltaTime)
{
	//ランダムで移動量
	GE::Math::Vector2 randVel = GE::Math::Vector2(range.x > 0 ? GE::RandomMaker::GetFloat(-range.x, range.x) : 0, range.y > 0 ? GE::RandomMaker::GetFloat(-range.y, range.y) : 0);
	//移動量をセットする
	cameraShake = GE::Math::Vector3(randVel.x, randVel.y, 0);
	//カメラシェイクの減衰
	range.x > 0 ? range.x -= deltaTime * GE::GameSetting::Time::GetGameTime() : range.x = 0.0f;
	range.y > 0 ? range.y -= deltaTime * GE::GameSetting::Time::GetGameTime() : range.y = 0.0f;
}
void CameraControl::ShakeStart(GE::Math::Vector2 range, int flame)
{
	this->range = range;
	//shakeFlame = flame;
}

void CameraControl::SetClearCameraPosition()
{
	graphicsDevice->GetMainCamera()->SetPosition({ 2931, 9476, -7104 });
}

void CameraControl::SetOverCameraPosition()
{
	//カメラのスタート位置
	//graphicsDevice->GetMainCamera()->SetPosition({ -1800,50,-9107 });
	//graphicsDevice->GetMainCamera()->SetPosition({ -300,9472,-9107 });
	graphicsDevice->GetMainCamera()->SetPosition({ -300,10072,-9107 });
}

//EaseIn関係がよくわからなかったから一時的に追加
const float CameraControl::easeIn(const float start, const float end, float time)
{
	return start * (1.0f - time * time) + end * time * time;
}

void CameraControl::SetGraphicsDevice(GE::IGraphicsDeviceDx12* graphicsDevice)
{
	this->graphicsDevice = graphicsDevice;
	position.x = graphicsDevice->GetMainCamera()->GetCameraInfo().cameraPos.x;
	position.y = graphicsDevice->GetMainCamera()->GetCameraInfo().cameraPos.y;
	position.z = graphicsDevice->GetMainCamera()->GetCameraInfo().cameraPos.z;
}