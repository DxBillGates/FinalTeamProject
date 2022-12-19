#include "CameraControl.h"
#include "PlayerComponent.h"
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

	range = {};
	cameraShake = {};

	nowProduct = 0;
	//レーン追加
	//鳥の前後がX軸、上下がY軸、左右がZ軸
	AddLane({ 1200,9000,-9700 }, { -1200,10000,-9700 });
	AddLane({ 400,6000,-9000 }, { 100,10000,-10500 });
	AddLane({ 1000,9000,-6000 }, { 1000,10600,-10000 });
	//カメラの開始位置を初めの始点に変更
	position = pCam[nowProduct].start;
	graphicsDevice->GetMainCamera()->SetPosition(position);

	//graphicsDevice->GetMainCamera()->SetPosition({ 0,5000,-20000 });
}

void CameraControl::Update()
{
	UIObject::GetInstance()->cameraPosition = position;
	//ターゲトポジションセット
	target = targetObject->GetTransform()->position;

	//体の角度計算
	if (abs(targetObject->GetTransform()->GetForward().y) < 0.6)
	{
		dir = atan2f(targetObject->GetTransform()->GetForward().x, targetObject->GetTransform()->GetForward().z);
	}

	//カメラの更新
	GE::Math::Vector3 newCameraPosition;
	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE)
	{
		current_cameraDistance = 2000;
		//newCameraPosition = target
		//	- GE::Math::Vector3(targetObject->GetTransform()->GetForward().x * current_cameraDistance,
		//		-300,
		//		targetObject->GetTransform()->GetForward().z * current_cameraDistance);
					//現在地から終点まで
		GE::Math::Vector3 num = pCam[nowProduct].end - position;
		num = num.Normalize();
		newCameraPosition = position + num * pCam[nowProduct].speed;

	}
	else if (PlayerComponent::statas == PlayerComponent::PlayerStatas::STAY_TREE)
	{
		current_cameraDistance = 1000;

		newCameraPosition = target
			- GE::Math::Vector3(targetObject->GetTransform()->GetForward().x * current_cameraDistance,
				-300,
				targetObject->GetTransform()->GetForward().z * current_cameraDistance);
	}
	else
	{
		current_cameraDistance = normal_cameraDistance;
		newCameraPosition = target - targetObject->GetTransform()->GetForward() * current_cameraDistance;
	}
	auto camera = dynamic_cast<GE::Camera3DDebug*>(graphicsDevice->GetMainCamera());
	GE::Math::Vector3 beforeCameraPosition = {
		camera->GetCameraInfo().cameraPos.x,
		camera->GetCameraInfo().cameraPos.y,
		camera->GetCameraInfo().cameraPos.z,
	};

	//カメラの向き
	GE::Math::Vector3 direction = GE::Math::Vector3(target - position).Normalize();

	float LERP_VALUE = 0.02f * GE::GameSetting::Time::GetGameTime();
	//ダッシュ時
	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::DASH
		|| PlayerComponent::statas == PlayerComponent::PlayerStatas::LOCKON_SHOOT)
	{
		LERP_VALUE = 0.035f * GE::GameSetting::Time::GetGameTime();
	}

	position = GE::Math::Vector3::Lerp(beforeCameraPosition, newCameraPosition, LERP_VALUE) + cameraShake * GE::GameSetting::Time::GetGameTime();

	//タイトルのカメラ
	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::TITLE) {
		//終点付近についたか(数値は誤差
		if (position.x <= pCam[nowProduct].end.x + 5 && position.x >= pCam[nowProduct].end.x - 5
			&& position.y <= pCam[nowProduct].end.y + 5 && position.y >= pCam[nowProduct].end.y - 5
			&& position.z <= pCam[nowProduct].end.z + 5 && position.z >= pCam[nowProduct].end.z - 5)
		{
			//レーン移動
			nowProduct = (nowProduct + 1) % pCam.size();
			//カメラの位置を次の開始位置に飛ばす
			position = pCam[nowProduct].start;
			graphicsDevice->GetMainCamera()->SetPosition(position);
		}
	}

	target += cameraShake * GE::GameSetting::Time::GetGameTime();
	//カメラシェイク
	Shake();

	camera->SetDirection(direction);
	camera->SetPosition(position);
}

void CameraControl::DashCam(float dashEasingCount, float dash_time)
{
	////カメラの距離遷移、遷移で離れて遷移で元に戻る
	//current_cameraDistance = easeIn(normal_cameraDistance, dash_cameraDistance,
	//	sin(GE::Math::Easing::Lerp(0, 3.14, dashEasingCount / dash_time)));
}

void CameraControl::AddLane(GE::Math::Vector3 start, GE::Math::Vector3 end, float speed)
{
	pCam.push_back(productionCamera());
	pCam.back().start = start;
	pCam.back().end = end;
	pCam.back().speed = speed;
}

void CameraControl::Shake()
{
	//ランダムで移動量
	GE::Math::Vector2 randVel = GE::Math::Vector2(range.x > 0 ? GE::RandomMaker::GetFloat(-range.x, range.x) : 0, range.y > 0 ? GE::RandomMaker::GetFloat(-range.y, range.y) : 0);
	//移動量をセットする
	cameraShake = GE::Math::Vector3(randVel.x, randVel.y, 0);
	//カメラシェイクの減衰
	range.x > 0 ? range.x -= 1.0 * GE::GameSetting::Time::GetGameTime() : range.x = 0.0f;
	range.y > 0 ? range.y -= 1.0f * GE::GameSetting::Time::GetGameTime() : range.y = 0.0f;
}
void CameraControl::ShakeStart(GE::Math::Vector2 range, int flame)
{
	this->range = range;
	//shakeFlame = flame;
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