#pragma once
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/Util/Math/Vector3.h>

class InputManager
{
private:
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

	GE::InputDevice* pInputDevice;
	GE::Keyboard* keyboard;
	GE::XInputController* xctrl;
	GE::Joycon* joyconL;
	GE::Joycon* joyconR;

	InputDeviceState currentInputDeviceState;

public:
	static InputManager* GetInstance();

	void Initialize();
	void Update();

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

	/// <summary>
	/// キーボードのSPACE / XコントローラーのA / ジョイコンのB
	/// </summary>
	/// <returns></returns>
	bool GetActionButton();
private:
	InputManager();
	InputManager(const InputManager&) = delete;
	~InputManager() = default;
	InputManager& operator=(const InputManager&) = delete;
};