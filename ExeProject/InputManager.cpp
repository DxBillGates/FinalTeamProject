#include "InputManager.h"
#include <GatesEngine/Header/Graphics/Window.h>

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

	isChangedInputDeviceFlagController.Initialize();
	isChangedInputDeviceFlagController.SetTime(1);
}

void InputManager::Update(float deltaTime)
{
	if (pInputDevice == nullptr)return;

	auto beforeInputDeviceState = currentInputDeviceState;

	if (keyboard->CheckHitKeyAll() == true)currentInputDeviceState = InputDeviceState::KEYBOARD;

	if (xctrl->CheckHitAllButton() == true)
	{
		currentInputDeviceState = InputDeviceState::XCTRL;
	}

	if (joyconL->GetAllButton() == true || joyconR->GetAllButton() == true)
	{
		currentInputDeviceState = InputDeviceState::JOYCON;
	}

	if (isChangedInputDeviceFlagController.GetOverTimeTrigger())
	{
		isChangedInputDeviceFlagController.SetFlag(false);
		isChangedInputDeviceFlagController.SetTime(1);
	}

	if (beforeInputDeviceState != currentInputDeviceState)
	{
		isChangedInputDeviceFlagController.SetFlag(true);
		isChangedInputDeviceFlagController.SetTime(0);
		isChangedInputDeviceFlagController.SetMaxTimeProperty(3);
	}

	isChangedInputDeviceFlagController.Update(deltaTime);
}

InputManager::Vector3 InputManager::GetAxis(int ctrlAxisIndex, InputCtrlAxisState ctrlAxisState)
{
	Vector3 result;

	GE::Math::Vector2 stick;

	switch (currentInputDeviceState)
	{
	case InputManager::InputDeviceState::KEYBOARD:
		if (keyboard->CheckHitKey(GE::Keys::W) || keyboard->CheckHitKey(GE::Keys::UP))result.y += 1;
		if (keyboard->CheckHitKey(GE::Keys::A) || keyboard->CheckHitKey(GE::Keys::LEFT))result.x += -1;
		if (keyboard->CheckHitKey(GE::Keys::S) || keyboard->CheckHitKey(GE::Keys::DOWN))result.y += -1;
		if (keyboard->CheckHitKey(GE::Keys::D) || keyboard->CheckHitKey(GE::Keys::RIGHT))result.x += 1;
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
			0,0,0
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

GE::Math::Vector3 InputManager::GetTriggerDirection()
{
	Vector3 result;

	switch (currentInputDeviceState)
	{
	case InputManager::InputDeviceState::KEYBOARD:
		if (keyboard->CheckPressTrigger(GE::Keys::UP))result.y += 1;
		if (keyboard->CheckPressTrigger(GE::Keys::LEFT))result.x += -1;
		if (keyboard->CheckPressTrigger(GE::Keys::DOWN))result.y += -1;
		if (keyboard->CheckPressTrigger(GE::Keys::RIGHT))result.x += 1;
		break;
	case InputManager::InputDeviceState::XCTRL:
		if (xctrl->CheckHitButtonTrigger(GE::XInputControllerButton::XINPUT_UP))result.y += 1;
		if (xctrl->CheckHitButtonTrigger(GE::XInputControllerButton::XINPUT_LEFT))result.x += -1;
		if (xctrl->CheckHitButtonTrigger(GE::XInputControllerButton::XINPUT_DOWN))result.y += -1;
		if (xctrl->CheckHitButtonTrigger(GE::XInputControllerButton::XINPUT_RIGHT))result.x += 1;
		break;
	case InputManager::InputDeviceState::JOYCON:
		if (joyconL->GetTriggerButton(GE::JoyconButtonData::UP))result.y += 1;
		if (joyconL->GetTriggerButton(GE::JoyconButtonData::LEFT))result.x += -1;
		if (joyconL->GetTriggerButton(GE::JoyconButtonData::DOWN))result.y += -1;
		if (joyconL->GetTriggerButton(GE::JoyconButtonData::RIGHT))result.x += 1;
		break;
	}

	return result;
}

bool InputManager::GetActionButton(bool isJoyconAcc)
{
	switch (currentInputDeviceState)
	{
	case InputManager::InputDeviceState::KEYBOARD:
		if (keyboard->CheckPressTrigger(GE::Keys::SPACE))return true;
		break;
	case InputManager::InputDeviceState::XCTRL:
		if (xctrl->CheckHitButtonTrigger(GE::XInputControllerButton::XINPUT_A))return true;
		break;
	case InputManager::InputDeviceState::JOYCON:
		if (joyconR->GetTriggerButton(GE::JoyconButtonData::B) && isJoyconAcc == false)return true;

		GE::Math::Vector3 acc = joyconL->GetAccelerometer();
		Vector3 accVector = { (float)acc.x,(float)acc.y,(float)acc.z };
		if (accVector.Length() > 2 && joyconR->GetButton(GE::JoyconButtonData::R))return true;

		break;
	}
	return false;
}

bool InputManager::GetLockonButton()
{
	switch (currentInputDeviceState)
	{
	case InputManager::InputDeviceState::KEYBOARD:
		if (keyboard->CheckHitKey(GE::Keys::RETURN))return true;
		break;
	case InputManager::InputDeviceState::XCTRL:
		if (xctrl->CheckHitButton(GE::XInputControllerButton::XINPUT_RT))return true;
		break;
	case InputManager::InputDeviceState::JOYCON:
		if (joyconR->GetButton(GE::JoyconButtonData::ZR))return true;
		break;
	}
	return false;
}

InputManager::InputDeviceState InputManager::GetCurrentInputDeviceState()
{
	return currentInputDeviceState;
}

void InputManager::Draw(GE::IGraphicsDeviceDx12* graphicsDevice)
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	// レンダーキューを2d用に切り替える
	graphicsDevice->SetCurrentRenderQueue(false);

	// テクスチャ描画用のシェーダーをセット
	graphicsDevice->SetShader("DefaultSpriteWithTextureShader");

	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ 500 });
	auto windowSize = GE::Window::GetWindowSize();
	float yPos = GE::Math::Lerp(windowSize.y / 3, windowSize.y / 4,isChangedInputDeviceFlagController.GetTime());
	modelMatrix *= GE::Math::Matrix4x4::Translate({ windowSize.x / 2,yPos,0 });

	// 画像の色変えたりするよう
	GE::Material material;
	material.color = { 1,1,1,1 - isChangedInputDeviceFlagController.GetTime() };

	// 2d用のカメラ情報 基本的に買えなくてok
	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	// アニメーションの情報
	GE::TextureAnimationInfo textureAnimationInfo;

	// 画像の元サイズ
	textureAnimationInfo.textureSize = 1;
	// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
	// textureSizeと一緒にすると切り抜かれずに描画される
	textureAnimationInfo.clipSize = 1;
	// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
	textureAnimationInfo.pivot = 1;

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });

	switch (currentInputDeviceState)
	{
	case InputManager::InputDeviceState::KEYBOARD:
		renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("keyboard_tex")->GetSRVNumber()});
		break;
	case InputManager::InputDeviceState::XCTRL:
		renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("xinput_tex")->GetSRVNumber() });
		break;
	case InputManager::InputDeviceState::JOYCON:
		renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("joycon_tex")->GetSRVNumber() });
		break;
	default:
		break;
	}
	graphicsDevice->DrawMesh("2DPlane");
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
