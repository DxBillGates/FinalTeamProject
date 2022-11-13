#include "CameraControl.h"
#include "PlayerComponent.h"
#include <GatesEngine/Header/Util/Random.h           >


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
	normal_cameraDistance = 500;
	dash_cameraDistance = 700;
	current_cameraDistance = normal_cameraDistance;

	shakeFlame = 0;

	range = {};
	cameraShake = {};
}

void CameraControl::Update()
{
	auto camera = dynamic_cast<GE::Camera3DDebug*>(graphicsDevice->GetMainCamera());
	GE::Math::Vector3 beforeCameraPosition = {
		camera->GetCameraInfo().cameraPos.x,
		camera->GetCameraInfo().cameraPos.y,
		camera->GetCameraInfo().cameraPos.z,
	};

	//体の角度計算
	if (abs(targetObject->GetTransform()->GetForward().y) < 0.6)
	{
		dir = atan2f(targetObject->GetTransform()->GetForward().x, targetObject->GetTransform()->GetForward().z);
	}
	target = targetObject->GetTransform()->position;
	float dirY = atan2f(targetObject->GetTransform()->GetForward().z, targetObject->GetTransform()->GetForward().y);

	//auto newCameraPosition = target + GE::Math::Vector3(sin(dir + 3.14) * current_cameraDistance, -cos(dirY + 0.3) * current_cameraDistance, cos(dir + 3.14) * current_cameraDistance);
	auto newCameraPosition = target + GE::Math::Vector3(sin(dir + 3.14) * current_cameraDistance, 100, cos(dir + 3.14) * current_cameraDistance);

	GE::Math::Vector3 direction = GE::Math::Vector3(target - position).Normalize();

	const float LERP_VALUE = 0.05f * PlayerComponent::GameTime;
	position = GE::Math::Vector3::Lerp(beforeCameraPosition, newCameraPosition, LERP_VALUE) + cameraShake * PlayerComponent::GameTime;
	target += cameraShake * PlayerComponent::GameTime;

	Shake();

	camera->SetDirection(direction);
	camera->SetPosition(position);
}

void CameraControl::DashCam(float dashEasingCount, float dash_time)
{
	//カメラの距離遷移、遷移で離れて遷移で元に戻る
	current_cameraDistance = easeIn(normal_cameraDistance, dash_cameraDistance,
		sin(GE::Math::Easing::Lerp(0, 3.14, dashEasingCount / dash_time)));
}

void CameraControl::Shake()
{
	//ランダムで移動量
	GE::Math::Vector2 randVel = GE::Math::Vector2(range.x > 0 ? GE::RandomMaker::GetFloat(-range.x, range.x) : 0, range.y > 0 ? GE::RandomMaker::GetFloat(-range.y, range.y) : 0);
	//移動量をセットする
	cameraShake = GE::Math::Vector3(randVel.x, randVel.y, 0);
	//カメラシェイクの減衰
	range.x > 0 ? range.x -= 1.0 * PlayerComponent::GameTime : range.x = 0.0f;
	range.y > 0 ? range.y -= 1.0f * PlayerComponent::GameTime : range.y = 0.0f;
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