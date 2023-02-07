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

	Vector3 GetTriggerDirection();

	/// <summary>
	/// �L�[�{�[�h��SPACE / X�R���g���[���[��A / �W���C�R����B or �����x�Z���T�[
	/// </summary>
	/// <returns></returns>
	bool GetActionButton(bool isJoyconAcc = false);

	/// <summary>
	/// �L�[�{�[�h��ENTER / X�R���g���[���[��RT / �W���C�R����ZR
	/// </summary>
	/// <returns></returns>
	bool GetLockonButton();

	/// <summary>
	/// ���݂̓��̓f�o�C�X��������Ԃ�
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