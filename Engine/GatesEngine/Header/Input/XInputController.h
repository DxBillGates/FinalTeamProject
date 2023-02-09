#pragma once
#include "XInputControllerButton.h"
#include "..\..\Header\Util\Math\Vector2.h"
#include "..\GameFramework\FlagController.h"
#include <Windows.h>
#include <Xinput.h>

namespace GE
{
	class XInputController
	{
	private:
		struct CheckButton
		{
			bool flag;
			XInputControllerButton button;
		};
	private:
		static int nextPadIndex;
		int mIndex;
		XINPUT_STATE ctrlState;
		CheckButton oldKey[16];

		FlagController vibrationFlagControler;
		float vibrationPower;
	public:
		XInputController(); //���g�ŏ���ɃR���g���[���[�̔ԍ������߂Đ���
		~XInputController();
		void Initialize();
		void Update();
		bool CheckHitAllButton();
		bool CheckHitButton(XInputControllerButton button);
		bool CheckHitButtonTrigger(XInputControllerButton button);
		float GetRStickX();
		float GetRStickY();
		float GetLStickX();
		float GetLStickY();
		Math::Vector2 GetRStick();
		Math::Vector2 GetLStick();
		void Vibration(float sec,float power = 1);
	};
}