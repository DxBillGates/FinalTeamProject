#include "..\..\Header\Input\InputDevice.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"Xinput.lib")

GE::InputDevice* GE::InputDevice::GetInstance()
{
	static InputDevice instance;
	return &instance;
}

void GE::InputDevice::Create(HWND hwnd, HINSTANCE hInstance)
{
	this->hwnd = hwnd;
	if (dInput != nullptr)return;
	HRESULT result;
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, nullptr);
	keyboard.Create(dInput, hwnd);
	mouse.Create(dInput, hwnd);
}

void GE::InputDevice::Initialize()
{
	keyboard.Initialize();
	mouse.Initialize();
	xctrler.Initialize();
	joyconL.SetIMU(true);
	joyconR.SetIMU(true);
}

void GE::InputDevice::Update()
{
	keyboard.Update();
	mouse.Update();
	xctrler.Update();
	joyconL.Update();
	joyconR.Update();
}

GE::Keyboard* GE::InputDevice::GetKeyboard()
{
	return &keyboard;
}

GE::Mouse* GE::InputDevice::GetMouse()
{
	return &mouse;
}

GE::XInputController* GE::InputDevice::GetXCtrler()
{
	return &xctrler;
}

GE::Joycon* GE::InputDevice::GetJoyconL()
{
	return &joyconL;
}

GE::Joycon* GE::InputDevice::GetJoyconR()
{
	return &joyconR;
}

bool GE::InputDevice::GetIsFocus()
{
	if (hwnd != GetFocus())return false;
	return true;
}

GE::InputDevice::InputDevice()
	:dInput(nullptr)
	, keyboard(Keyboard())
	, mouse(Mouse())
	, hwnd(HWND())
	, xctrler(XInputController())
	, joyconL(Joycon(JoyconType::L))
	, joyconR(Joycon(JoyconType::R))
{
}
