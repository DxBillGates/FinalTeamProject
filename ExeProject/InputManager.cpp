#include "InputManager.h"

InputManager* InputManager::GetInstance()
{
	static InputManager instance;
	return &instance;
}

void InputManager::Initialize()
{
	pInputDevice = GE::InputDevice::GetInstance();
	keyboard = pInputDevice->GetKeyboard();
	xctrl = pInputDevice->GetXCtrler();
	joyconL = pInputDevice->GetJoyconL();
	joyconR = pInputDevice->GetJoyconR();
}

void InputManager::Update()
{
	if (pInputDevice == nullptr)return;

	if (keyboard->CheckHitKeyAll() == true)currentInputDeviceState = InputDeviceState::KEYBOARD;

	if (xctrl->CheckHitAllButton() == true)
	{
		currentInputDeviceState = InputDeviceState::XCTRL;
	}

	if (joyconL->GetAllButton() == true || joyconR->GetAllButton() == true)
	{
		currentInputDeviceState = InputDeviceState::JOYCON;
	}
}

InputManager::Vector3 InputManager::GetAxis(int ctrlAxisIndex, InputCtrlAxisState ctrlAxisState)
{
	Vector3 result;

	GE::Math::Vector2 stick;

	switch (currentInputDeviceState)
	{
	case InputManager::InputDeviceState::KEYBOARD:
		if (keyboard->CheckHitKey(GE::Keys::W))result.y += 1;
		if (keyboard->CheckHitKey(GE::Keys::A))result.x += -1;
		if (keyboard->CheckHitKey(GE::Keys::S))result.y += -1;
		if (keyboard->CheckHitKey(GE::Keys::D))result.x += 1;
		break;
	case InputManager::InputDeviceState::XCTRL:
		stick = ctrlAxisIndex == 0 ? xctrl->GetLStick() : xctrl->GetRStick();
		result = { stick.x,stick.y,0 };
		break;
	case InputManager::InputDeviceState::JOYCON:
		stick = ctrlAxisIndex == 0 ? GE::Math::Vector2(joyconL->GetStick().x, joyconL->GetStick().y)
			: GE::Math::Vector2(joyconR->GetStick().x, joyconR->GetStick().y);

		auto joyconLGyro = joyconL->GetGyroscope();
		auto joyconRGyro = joyconR->GetGyroscope();
		GE::Math::Vector3 gyroAxis =
		{
			(joyconLGyro.x + joyconRGyro.x) / 2.0f,
			(joyconLGyro.y + joyconRGyro.y) / 2.0f,
			(joyconLGyro.z + joyconRGyro.z) / 2.0f
		};

		if (ctrlAxisState == InputCtrlAxisState::STICK)
		{
			result.x = stick.x;
			result.y = stick.y;
		}
		else
		{
			result = gyroAxis;
		}

		break;
	}

	return result;
}

InputManager::Vector3 InputManager::GetDirection()
{
	Vector3 result;

	switch (currentInputDeviceState)
	{
	case InputManager::InputDeviceState::KEYBOARD:
		if (keyboard->CheckHitKey(GE::Keys::UP))result.y += 1;
		if (keyboard->CheckHitKey(GE::Keys::LEFT))result.x += -1;
		if (keyboard->CheckHitKey(GE::Keys::DOWN))result.y += -1;
		if (keyboard->CheckHitKey(GE::Keys::RIGHT))result.x += 1;
		break;
	case InputManager::InputDeviceState::XCTRL:
		if (xctrl->CheckHitButton(GE::XInputControllerButton::XINPUT_UP))result.y += 1;
		if (xctrl->CheckHitButton(GE::XInputControllerButton::XINPUT_LEFT))result.x += -1;
		if (xctrl->CheckHitButton(GE::XInputControllerButton::XINPUT_DOWN))result.y += -1;
		if (xctrl->CheckHitButton(GE::XInputControllerButton::XINPUT_RIGHT))result.x += 1;
		break;
	case InputManager::InputDeviceState::JOYCON:
		if (joyconL->GetButton(GE::JoyconButtonData::UP))result.y += 1;
		if (joyconL->GetButton(GE::JoyconButtonData::LEFT))result.x += -1;
		if (joyconL->GetButton(GE::JoyconButtonData::DOWN))result.y += -1;
		if (joyconL->GetButton(GE::JoyconButtonData::RIGHT))result.x += 1;
		break;
	}

	return result;
}

bool InputManager::GetActionButton()
{
	switch (currentInputDeviceState)
	{
	case InputManager::InputDeviceState::KEYBOARD:
		if (keyboard->CheckHitKey(GE::Keys::SPACE))return true;
		break;
	case InputManager::InputDeviceState::XCTRL:
		if (xctrl->CheckHitButton(GE::XInputControllerButton::XINPUT_A))return true;
		break;
	case InputManager::InputDeviceState::JOYCON:
		if (joyconR->GetButton(GE::JoyconButtonData::B))return true;
		break;
	}
	return false;
}

InputManager::InputManager()
	: pInputDevice(nullptr)
	, keyboard(nullptr)
	, xctrl(nullptr)
	, joyconL(nullptr)
	, joyconR(nullptr)
	, currentInputDeviceState(InputDeviceState::KEYBOARD)
{

}
