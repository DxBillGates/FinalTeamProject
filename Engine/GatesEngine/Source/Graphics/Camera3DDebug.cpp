#include "..\..\Header\Graphics\Camera3DDebug.h"
#include "..\..\Header\Util\Math\Math.h"
#include "..\..\Header\Graphics\Window.h"

GE::Camera3DDebug::Camera3DDebug()
	: keyboard(nullptr)
	, mouse(nullptr)
	, moveSpeed(1)
{
}

GE::Camera3DDebug::~Camera3DDebug()
{
}

void GE::Camera3DDebug::Initialize()
{
	Camera3D::Initialize();
	keyboard = InputDevice::GetInstance()->GetKeyboard();
	mouse = InputDevice::GetInstance()->GetMouse();
	moveSpeed = 1;
}

void GE::Camera3DDebug::Update()
{
	Math::Axis axis = rotation.GetAxis();

	float wheelValue = mouse->GetWheelValue();
	Math::Vector3 moveVector;
	if (mouse->GetCheckHitButton(MouseButtons::RIGHT_CLICK))
	{
		windowSize = GE::Window::GetWindowSize();
		mouse->SetMouseCursor(windowSize / 2);
		//マウスの移動量を取得
		Math::Vector2 inputValue = mouse->GetMouseMove() / 500.0f;

		pitch -= inputValue.y;
		yaw -= inputValue.x;

		float minMaxPitch = Math::ConvertToRadian(89);
		if (pitch > minMaxPitch)pitch = minMaxPitch;
		if (pitch < -minMaxPitch)pitch = -minMaxPitch;


		if (keyboard->CheckHitKey(Keys::W))
		{
			moveVector += axis.z;
		}
		if (keyboard->CheckHitKey(Keys::S))
		{
			moveVector -= axis.z;
		}
		if (keyboard->CheckHitKey(Keys::D))
		{
			moveVector += axis.x;
		}
		if (keyboard->CheckHitKey(Keys::A))
		{
			moveVector -= axis.x;
		}
		if (keyboard->CheckHitKey(Keys::E))
		{
			moveVector += axis.y;
		}
		if (keyboard->CheckHitKey(Keys::Q))
		{
			moveVector -= axis.y;
		}
		moveSpeed = keyboard->CheckHitKey(Keys::LSHIFT) ? 100.0f : 1.0f;

		moveVector = moveVector.Normalize();
		moveVector += wheelValue * axis.z.Normalize();

		direction = Math::Vector3(0, 0, 1);
		rotation = Math::Matrix4x4::RotationX(pitch) * Math::Matrix4x4::RotationY(yaw);
		direction = Math::Matrix4x4::Transform(direction, rotation);
		info.cameraPos += moveVector * moveSpeed;
	}
	Math::Vector3 pos = { info.cameraPos.x,info.cameraPos.y ,info.cameraPos.z };

	info.projMatrix = Math::Matrix4x4::GetPerspectiveMatrix(90, aspect, 1, 60000 * 5);
	info.viewMatrix = Math::Matrix4x4::GetViewMatrixLookTo(pos, direction, axis.y);
	info.invViewMatrix = Math::Matrix4x4::Inverse(info.viewMatrix);
	info.invProjMatrix = Math::Matrix4x4::Inverse(info.projMatrix);
	info.cameraPos = { pos.x,pos.y,pos.z,1 };
	info.cameraDir = { direction.x,direction.y,direction.z,1 };
}
