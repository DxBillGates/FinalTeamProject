#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Keyboard.h"
#include "Mouse.h"
#include "XInputController.h"
#include "Joycon.h"

#include <dinput.h>
#include <Xinput.h>

namespace GE
{
	//キーボードやマウスといった入力関係を管理するシングルトンクラス
	class InputDevice
	{
	private:
		HWND hwnd;
		IDirectInput8* dInput;
		Keyboard keyboard;
		Mouse mouse;
		XInputController xctrler;
		//DCtrler dctrler;
		Joycon joyconL;
		Joycon joyconR;
	public:
		/// <summary>
		/// シングルトンのポインタを返す関数
		/// </summary>
		/// <returns>シングルトンのポインタ</returns>
		static InputDevice* GetInstance();

		/// <summary>
		/// 各メンバを生成する関数
		/// </summary>
		/// <param name="hwnd">使用しているWindowのハンドル</param>
		/// <param name="hInstance">使用しているWindowのインスタンスハンドル</param>
		void Create(HWND hwnd, HINSTANCE hInstance);

		/// <summary>
		/// 各デバイスの初期化をする関数
		/// </summary>
		void Initialize();

		/// <summary>
		/// 各デバイスの更新をする関数
		/// </summary>
		void Update();

		/// <summary>
		/// キーボードの情報を保持しているクラスのポインタを返す関数
		/// </summary>
		/// <returns>キーボードの情報を保持しているクラスのポインタ</returns>
		Keyboard* GetKeyboard();

		/// <summary>
		/// マウス情報を保持しているクラスのポインタを返す関数
		/// </summary>
		/// <returns>マウス情報を保持しているクラスのポインタ</returns>
		Mouse* GetMouse();

		/// <summary>
		/// 接続されているXInputControllerを返す
		/// </summary>
		/// <returns></returns>
		XInputController* GetXCtrler();

		Joycon* GetJoyconL();
		Joycon* GetJoyconR();

		/// <summary>
		/// 使用しているWindowにフォーカスがあっているか確認する関数
		/// </summary>
		/// <returns>成否</returns>
		bool GetIsFocus();

		//XCtrler* GetXInputCtrler();
		//DCtrler* GetDInputCtrler();
	protected:
		InputDevice();
		InputDevice(const InputDevice&) = delete;
		~InputDevice() = default;
		InputDevice& operator=(const InputDevice&) = delete;
	};
}