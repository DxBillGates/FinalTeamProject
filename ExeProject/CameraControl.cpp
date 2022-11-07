#include "CameraControl.h"
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
	dir = 0.0f;

	shakeFlame = 0;
	count = 0;
	isShake = false;
	range = {};
}

void CameraControl::Update()
{
	auto camera = dynamic_cast<GE::Camera3DDebug*>(graphicsDevice->GetMainCamera());
	GE::Math::Vector3 beforeCameraPosition = {
		camera->GetCameraInfo().cameraPos.x,
		camera->GetCameraInfo().cameraPos.y,
		camera->GetCameraInfo().cameraPos.z,
	};
	auto newCameraPosition = other + GE::Math::Vector3(sin(dir + 3.14) * current_cameraDistance, 100, cos(dir + 3.14) * current_cameraDistance);
	camera->SetDirection(direction);

	const float LERP_VALUE = 0.05f;

	//GE::Math::Vector3 behind = other - otherAxis.z * normal_cameraDistance;
	
	/*position = GE::Math::Vector3(behind.x
		, GE::Math::Lerp(beforeCameraPosition.y, newCameraPosition.y, LERP_VALUE)
		, behind.z);*/

	position = GE::Math::Vector3::Lerp(beforeCameraPosition, newCameraPosition, LERP_VALUE);

	Shake();

	camera->SetPosition(position);
}

void CameraControl::Direction(GE::Math::Vector3& target)
{
	direction.x = target.x - position.x;
	direction.y = target.y - position.y;
	direction.z = target.z - position.z;
	direction = direction.Normalize();
}

void CameraControl::DashCam(float dashEasingCount, float dash_time)
{
	//ƒJƒƒ‰‚Ì‹——£‘JˆÚA‘JˆÚ‚Å—£‚ê‚Ä‘JˆÚ‚ÅŒ³‚É–ß‚é
	current_cameraDistance = easeIn(normal_cameraDistance, dash_cameraDistance,
		sin(GE::Math::Easing::Lerp(0, 3.14, dashEasingCount / dash_time)));
}

void CameraControl::Shake()
{
	if (isShake)
	{
		if (shakeFlame > count)
		{
			if (count % 2 == 0)
			{
				position = { position.x + range.x,position.y + range.y,position.z };
			}
			else
			{
				position = { position.x - range.x,position.y - range.y,position.z };
			}

			//GE::Math::Vector2 random;
			//random = { GE::RandomMaker::GetFloat(-range.x,range.x),
			//	GE::RandomMaker::GetFloat(-range.y,range.y) };//ˆÚ“®•
			//position = { position.x + random.x,position.y + random.y,position.z };

			count++;
		}
		else
		{
			count = 0;
			isShake = false;
		}
	}
}

void CameraControl::ShakeStart(GE::Math::Vector2 range, int flame)
{
	isShake = true;
	this->range = range;
	if (range.x < 0)
	{
		this->range.x *= -1;
	}
	if (range.y < 0)
	{
		this->range.y *= -1;
	}
	shakeFlame = flame;
}

//EaseInŠÖŒW‚ª‚æ‚­‚í‚©‚ç‚È‚©‚Á‚½‚©‚çˆêŽž“I‚É’Ç‰Á
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

void CameraControl::SetOtherPos(GE::Math::Vector3& pos)
{
	other = pos;
}

void CameraControl::SetOtherAxis(GE::Math::Axis axis)
{
	otherAxis = axis;
}
