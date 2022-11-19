#include "CameraControl.h"
#include "PlayerComponent.h"
#include <GatesEngine/Header\GameFramework/GameSetting.h>
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
	normal_cameraDistance = 300;
	dash_cameraDistance = 500;
	current_cameraDistance = normal_cameraDistance;

	shakeFlame = 0;

	range = {};
	cameraShake = {};

	graphicsDevice->GetMainCamera()->SetPosition({ 0,5000,-20000 });
}

void CameraControl::Update()
{
	//�^�[�Q�g�|�W�V�����Z�b�g
	target = targetObject->GetTransform()->position;
	//�̂̊p�x�v�Z
	if (abs(targetObject->GetTransform()->GetForward().y) < 0.6)
	{
		dir = atan2f(targetObject->GetTransform()->GetForward().x, targetObject->GetTransform()->GetForward().z);
	}

	//�J�����̍X�V
	GE::Math::Vector3 newCameraPosition;
	if (targetObject->GetComponent<PlayerComponent>()->statas == PlayerComponent::PlayerStatas::STAY_LAND)
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

	//�J�����̌���
	GE::Math::Vector3 direction = GE::Math::Vector3(target - position).Normalize();

	float LERP_VALUE = 0.01f * GE::GameSetting::Time::GetGameTime();
	//�_�b�V����
	if (targetObject->GetComponent<PlayerComponent>()->statas == PlayerComponent::PlayerStatas::DASH
		|| targetObject->GetComponent<PlayerComponent>()->statas == PlayerComponent::PlayerStatas::LOCKON_SHOOT)
	{
		LERP_VALUE = 0.035f * GE::GameSetting::Time::GetGameTime();
	}
	position = GE::Math::Vector3::Lerp(beforeCameraPosition, newCameraPosition, LERP_VALUE) + cameraShake * GE::GameSetting::Time::GetGameTime();
	target += cameraShake * GE::GameSetting::Time::GetGameTime();
	//�J�����V�F�C�N
	Shake();

	camera->SetDirection(direction);
	camera->SetPosition(position);
}

void CameraControl::DashCam(float dashEasingCount, float dash_time)
{
	////�J�����̋����J�ځA�J�ڂŗ���đJ�ڂŌ��ɖ߂�
	//current_cameraDistance = easeIn(normal_cameraDistance, dash_cameraDistance,
	//	sin(GE::Math::Easing::Lerp(0, 3.14, dashEasingCount / dash_time)));
}

void CameraControl::Shake()
{
	//�����_���ňړ���
	GE::Math::Vector2 randVel = GE::Math::Vector2(range.x > 0 ? GE::RandomMaker::GetFloat(-range.x, range.x) : 0, range.y > 0 ? GE::RandomMaker::GetFloat(-range.y, range.y) : 0);
	//�ړ��ʂ��Z�b�g����
	cameraShake = GE::Math::Vector3(randVel.x, randVel.y, 0);
	//�J�����V�F�C�N�̌���
	range.x > 0 ? range.x -= 1.0 * GE::GameSetting::Time::GetGameTime() : range.x = 0.0f;
	range.y > 0 ? range.y -= 1.0f * GE::GameSetting::Time::GetGameTime() : range.y = 0.0f;
}
void CameraControl::ShakeStart(GE::Math::Vector2 range, int flame)
{
	this->range = range;
	//shakeFlame = flame;
}

//EaseIn�֌W���悭�킩��Ȃ���������ꎞ�I�ɒǉ�
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