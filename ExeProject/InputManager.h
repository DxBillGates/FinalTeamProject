#pragma once
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/Util/Math/Math.h>
#include <GatesEngine/Header/Graphics/IGraphicsDeviceDx12.h>
#include <GatesEngine/Header/GameFramework/FlagController.h>

class InputManager
{
public:
	enum class InputDeviceState
	{
		KEYBOARD,
		XCTRL,
		JOYCON,
	};

	enum class InputCtrlAxisState
	{
		STICK,
		GYROSCOPE,
		//ACCLEROMETR,
	};
private:
	using Vector3 = GE::Math::Vector3;
	using Quaternion = GE::Math::Quaternion;

	GE::InputDevice* pInputDevice;
	GE::Keyboard* keyboard;
	GE::XInputController* xctrl;
	GE::Joycon* joyconL;
	GE::Joycon* joyconR;

	InputDeviceState currentInputDeviceState;

	GE::FlagController isChangedInputDeviceFlagController;

public:
	static InputManager* GetInstance();

	void Initialize();
	void Update(float deltaTime);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>

	/// <summary>
	/// <para>キーボードのWASD / コントローラーの左スティック /
	/// コントローラーの右スティック / コントローラーのジャイロ、加速センサーを取得
	/// </para>
	/// <para>
	/// コントローラーの情報を取る際はどの軸データを取るかを引数で指定して使う
	/// </para>
	/// </summary>
	/// <param name="ctrlAxisIndex">コントローラーのスティック番号 0 = left / 1 = right</param>
	/// <param name="ctrlAxisState">コントローラーからどのデータを取るか</param>
	/// <returns></returns>
	Vector3 GetAxis(int ctrlAxisIndex = 0, InputCtrlAxisState ctrlAxisState = InputCtrlAxisState::STICK);

	/// <summary>
	/// キーボードのアローキー / コントローラーの十字キー
	/// </summary>
	/// <returns></returns>
	Vector3 GetDirection();

	Vector3 GetTriggerDirection();

	/// <summary>
	/// キーボードのSPACE / XコントローラーのA / ジョイコンのB or 加速度センサー
	/// </summary>
	/// <returns></returns>
	bool GetActionButton(bool isJoyconAcc = false);

	/// <summary>
	/// キーボードのENTER / XコントローラーのRT / ジョイコンのZR
	/// </summary>
	/// <returns></returns>
	bool GetLockonButton();

	/// <summary>
	/// 現在の入力デバイスが何かを返す
	/// </summary>
	/// <returns></returns>
	InputDeviceState GetCurrentInputDeviceState();

	void Draw(GE::IGraphicsDeviceDx12* graphicsDevice);
private:
	InputManager();
	InputManager(const InputManager&) = delete;
	~InputManager() = default;
	InputManager& operator=(const InputManager&) = delete;
};