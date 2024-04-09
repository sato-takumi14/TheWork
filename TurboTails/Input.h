#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* Input�N���X�F���͏����̔��������N���X
********************************************************************************************/

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "xinput.lib")

#include<windowsx.h>

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
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

		LPDIRECTINPUT8			mDInput;			// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
		LPDIRECTINPUTDEVICE8	mMouse;				// mouse
		DIMOUSESTATE2			mMouseState;		// �}�E�X�̃_�C���N�g�ȏ��
		DIMOUSESTATE2			mMouseTrigger;		// �����ꂽ�u�Ԃ���ON

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

	// XINPUT_GAMEPAD_DPAD_UP			�\���L�[��
	// XINPUT_GAMEPAD_DPAD_DOWN			�\���L�[��
	// XINPUT_GAMEPAD_DPAD_LEFT			�\���L�[��
	// XINPUT_GAMEPAD_DPAD_RIGHT		�\���L�[�E
	// XINPUT_GAMEPAD_START
	// XINPUT_GAMEPAD_BACK
	// XINPUT_GAMEPAD_LEFT_THUMB		L�X�e�B�b�N��������
	// XINPUT_GAMEPAD_RIGHT_THUMB		R�X�e�B�b�N��������
	// XINPUT_GAMEPAD_LEFT_SHOULDER		L�{�^��
	// XINPUT_GAMEPAD_RIGHT_SHOULDER	R�{�^��
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

	/*�L�[�{�[�h*/
	static Keyboard* GetKeyboard() { return &mKeyboard; }
	static cbool GetKeyPress(const BYTE& KeyCode);
	static cbool GetKeyTrigger(const BYTE& KeyCode);

	/*�}�E�X*/
	static Mouse* GetMouse() { return &mMouse; }
	static cbool IsMouseLeftPressed(const BYTE& KeyCode);		/*���N���b�N�������*/
	static cbool IsMouseLeftTriggered(const BYTE& KeyCode);		/*���N���b�N�����u��*/
	static cbool IsMouseRightPressed(const BYTE& KeyCode);		/*�E�N���b�N�������*/
	static cbool IsMouseRightTriggered(const BYTE& KeyCode);	/*�E�N���b�N�����u��*/
	static cbool IsMouseCenterPressed(const BYTE& KeyCode);		/*�}�E�X�z�C�[�����N���b�N�������*/
	static cbool IsMouseCenterTriggered(const BYTE& KeyCode);	/*�}�E�X�z�C�[�����N���b�N�����u��*/
	static const long GetMouseX();								/*�}�E�X��X�����ɓ��������Βl*/
	static const long GetMouseY();								/*�}�E�X��Y�����ɓ��������Βl*/
	static const long GetMouseZ();								/*�}�E�X�z�C�[�������������Βl*/
	static cver3 GetMouseVector3();

	/*�Q�[���p�b�h*/
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
