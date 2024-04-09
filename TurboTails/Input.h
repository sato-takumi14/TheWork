#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* Inputクラス：入力処理の判定をするクラス
********************************************************************************************/

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "xinput.lib")

#include<windowsx.h>

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"

#include<XInput.h>

class Input
{
private:

	class InputInterface
	{
	private:


	public:

		virtual void Init() {}
		virtual void Update() = 0;
		virtual void Uninit() {}

		virtual cbool Press(const BYTE& KeyCode)const = 0;
		virtual cbool Trigger(const BYTE& KeyCode)const = 0;

		virtual cver2 GetInputVector2()const { return cver2(); }
		virtual cver3 GetInputVector3()const { return cver3(); }

	};

	class Keyboard : public InputInterface
	{
	private:

		BYTE m_OldKeyState[256]{};
		BYTE m_KeyState[256]{};

	public:

		void Init()override;
		void Update()override;
		cbool Press(const BYTE& KeyCode)const override;
		cbool Trigger(const BYTE& KeyCode)const override;

	};

	class Mouse
	{
	private:

		LPDIRECTINPUT8			mDInput;			// IDirectInput8インターフェースへのポインタ
		LPDIRECTINPUTDEVICE8	mMouse;				// mouse
		DIMOUSESTATE2			mMouseState;		// マウスのダイレクトな状態
		DIMOUSESTATE2			mMouseTrigger;		// 押された瞬間だけON

	public:

		cbool Init(HINSTANCE hInst);
		void Update();
		void Uninit();
		
		cbool LeftPress()const;
		cbool LeftTrigger()const;

		cbool RightPress()const;
		cbool RightTrigger()const;

		cbool CenterPress()const;
		cbool CenterTrigger()const;

		const long GetX()const;
		const long GetY()const;
		const long GetZ()const;

		cver3 GetInputVector3()const;

	};

	typedef struct GamePadState {

		XINPUT_STATE		last;
		XINPUT_STATE		state;
		XINPUT_STATE		trigger;
		XINPUT_VIBRATION	vibration;

	}GamePadState;

	// XINPUT_GAMEPAD_DPAD_UP			十字キー上
	// XINPUT_GAMEPAD_DPAD_DOWN			十字キー下
	// XINPUT_GAMEPAD_DPAD_LEFT			十字キー左
	// XINPUT_GAMEPAD_DPAD_RIGHT		十字キー右
	// XINPUT_GAMEPAD_START
	// XINPUT_GAMEPAD_BACK
	// XINPUT_GAMEPAD_LEFT_THUMB		Lスティック押し込み
	// XINPUT_GAMEPAD_RIGHT_THUMB		Rスティック押し込み
	// XINPUT_GAMEPAD_LEFT_SHOULDER		Lボタン
	// XINPUT_GAMEPAD_RIGHT_SHOULDER	Rボタン
	// XINPUT_GAMEPAD_A
	// XINPUT_GAMEPAD_B
	// XINPUT_GAMEPAD_X
	// XINPUT_GAMEPAD_Y

	class GamePad
	{
	private:

		GamePadState mState{};
		ver2 mLeft{};
		ver2 mRight{};
		float mMagnitude{};
		bool mConnect{};

	public:

		void Init();
		void Update();
		void Uninit();

		cbool Connect();
		cbool GetConnect()const { return mConnect; }

		cver2 GetLeft()const;
		cfloat GetLeftX()const;
		cfloat GetLeftY()const;

		cver2 GetRight()const;
		cfloat GetRightX()const;
		cfloat GetRightY()const;

		cint GetLeftTrigger()const;
		cint GetRightTrigger()const;

		void SetLeftVibration(crint speed);
		void SetRightVibration(crint speed);

		void SetVibration(crint speed);
		void StopVibration();

		cbool Press(const DWORD& button)const;
		cbool Trigger(const DWORD& button)const;

	};

private:

	static Keyboard mKeyboard;
	static Mouse mMouse;
	static GamePad mGamePad;

public:

	static void Init(HINSTANCE hInst, HWND hWindow);
	static void Uninit();
	static void Update();

	/*キーボード*/
	static Keyboard* GetKeyboard() { return &mKeyboard; }
	static cbool GetKeyPress(const BYTE& KeyCode);
	static cbool GetKeyTrigger(const BYTE& KeyCode);

	/*マウス*/
	static Mouse* GetMouse() { return &mMouse; }
	static cbool IsMouseLeftPressed(const BYTE& KeyCode);		/*左クリックした状態*/
	static cbool IsMouseLeftTriggered(const BYTE& KeyCode);		/*左クリックした瞬間*/
	static cbool IsMouseRightPressed(const BYTE& KeyCode);		/*右クリックした状態*/
	static cbool IsMouseRightTriggered(const BYTE& KeyCode);	/*右クリックした瞬間*/
	static cbool IsMouseCenterPressed(const BYTE& KeyCode);		/*マウスホイールをクリックした状態*/
	static cbool IsMouseCenterTriggered(const BYTE& KeyCode);	/*マウスホイールをクリックした瞬間*/
	static const long GetMouseX();								/*マウスがX方向に動いた相対値*/
	static const long GetMouseY();								/*マウスがY方向に動いた相対値*/
	static const long GetMouseZ();								/*マウスホイールが動いた相対値*/
	static cver3 GetMouseVector3();

	/*ゲームパッド*/
	static GamePad* GetGamePad() { return &mGamePad; }
	static cver2	GamePadGetLeft();
	static cfloat	GamePadGetLeftX();
	static cfloat	GamePadGetLeftY();
	static cver2	GamePadGetRight();
	static cfloat	GamePadGetRightX();
	static cfloat	GamePadGetRightY();
	static cint		GamePadGetLeftTrigger();
	static cint		GamePadGetRightTrigger();
	static void		GamePadSetLeftVibration(crint speed);
	static void		GamePadSetRightVibration(crint speed);
	static void		GamePadSetVibration(crint speed);
	static void		GamePadStopVibration();
	static cbool	GamePadPress(const DWORD& button);
	static cbool	GamePadTrigger(const DWORD& button);
	static cbool	Connect();

};
