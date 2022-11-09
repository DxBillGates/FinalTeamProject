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
	/// <para>�L�[�{�[�h��WASD / �R���g���[���[�̍��X�e�B�b�N /
	/// �R���g���[���[�̉E�X�e�B�b�N / �R���g���[���[�̃W���C���A�����Z���T�[���擾
	/// </para>
	/// <para>
	/// �R���g���[���[�̏������ۂ͂ǂ̎��f�[�^����邩�������Ŏw�肵�Ďg��
	/// </para>
	/// </summary>
	/// <param name="ctrlAxisIndex">�R���g���[���[�̃X�e�B�b�N�ԍ� 0 = left / 1 = right</param>
	/// <param name="ctrlAxisState">�R���g���[���[����ǂ̃f�[�^����邩</param>
	/// <returns></returns>
	Vector3 GetAxis(int ctrlAxisIndex = 0, InputCtrlAxisState ctrlAxisState = InputCtrlAxisState::STICK);

	/// <summary>
	/// �L�[�{�[�h�̃A���[�L�[ / �R���g���[���[�̏\���L�[
	/// </summary>
	/// <returns></returns>
	Vector3 GetDirection();

	/// <summary>
	/// �L�[�{�[�h��SPACE / X�R���g���[���[��A / �W���C�R����B
	/// </summary>
	/// <returns></returns>
	bool GetActionButton();
private:
	InputManager();
	InputManager(const InputManager&) = delete;
	~InputManager() = default;
	InputManager& operator=(const InputManager&) = delete;
};