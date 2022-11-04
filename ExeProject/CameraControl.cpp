#include "CameraControl.h"


//CameraControl* CameraControl::GetInstance()
//{
//	static CameraControl instance;
//	return &instance;
//}


void CameraControl::Create()
{
}

void CameraControl::Initialize()
{
	normal_cameraDistance = 500;
	dash_cameraDistance = 700;
	current_cameraDistance = normal_cameraDistance;
	dir = 0.0f;
}

void CameraControl::Update()
{

	auto camera = dynamic_cast<GE::Camera3DDebug*>(graphicsDevice->GetMainCamera());

	camera->SetDirection(direction);
	camera->SetPosition(other + GE::Math::Vector3(sin(dir + 3.14) * current_cameraDistance, 100, cos(dir + 3.14) * current_cameraDistance));
	//camera->SetPosition(other - otherAxis.z * current_cameraDistance);
		//更新した位置取得
	position.x = graphicsDevice->GetMainCamera()->GetCameraInfo().cameraPos.x;
	position.y = graphicsDevice->GetMainCamera()->GetCameraInfo().cameraPos.y;
	position.z = graphicsDevice->GetMainCamera()->GetCameraInfo().cameraPos.z;
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
	//カメラの距離遷移、遷移で離れて遷移で元に戻る
	current_cameraDistance = easeIn(normal_cameraDistance, dash_cameraDistance,
		sin(GE::Math::Easing::Lerp(0, 3.14, dashEasingCount / dash_time)));
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

void CameraControl::SetPosition(GE::Math::Vector3& pos)
{
	other = pos;
}

void CameraControl::SetOtherAxis(GE::Math::Axis axis)
{
	otherAxis = axis;
}
