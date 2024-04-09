
#include"Main.h"
#include "Input.h"
#include"Debug.h"

cint MaxButton = 4;

Input::Keyboard Input::mKeyboard{};
Input::Mouse	Input::mMouse{};
Input::GamePad	Input::mGamePad{};

void Input::Init(HINSTANCE hInst, HWND hWindow)
{

	mKeyboard.Init();
	mMouse.Init(hInst);
	mGamePad.Init();
	if (mGamePad.Connect())Debug::Log("Connect");
	else Debug::Log("Not Connect");

}

void Input::Uninit()
{

	mKeyboard.Uninit();
	mMouse.Uninit();
	mGamePad.Uninit();

}

void Input::Update()
{

	mKeyboard.Update();
	mMouse.Update();
	mGamePad.Update();

}

cbool Input::GetKeyPress(const BYTE& KeyCode)
{
	return mKeyboard.Press(KeyCode);
}

cbool Input::GetKeyTrigger(const BYTE& KeyCode)
{
	return mKeyboard.Trigger(KeyCode);
}

cbool Input::IsMouseLeftPressed(const BYTE& KeyCode)
{
	return mMouse.LeftPress();
}

cbool Input::IsMouseLeftTriggered(const BYTE& KeyCode)
{
	return mMouse.LeftTrigger();
}

cbool Input::IsMouseRightPressed(const BYTE& KeyCode)
{
	return mMouse.RightPress();
}

cbool Input::IsMouseRightTriggered(const BYTE& KeyCode)
{
	return mMouse.RightTrigger();
}

cbool Input::IsMouseCenterPressed(const BYTE& KeyCode)
{
	return mMouse.CenterPress();
}

cbool Input::IsMouseCenterTriggered(const BYTE& KeyCode)
{
	return mMouse.CenterTrigger();
}

const long Input::GetMouseX()
{
	return mMouse.GetX();
}

const long Input::GetMouseY()
{
	return mMouse.GetY();
}

const long Input::GetMouseZ()
{
	return mMouse.GetZ();
}

cver3 Input::GetMouseVector3()
{
	return mMouse.GetInputVector3();
}

cver2 Input::GamePadGetLeft()
{
	return mGamePad.GetLeft();
}

cfloat Input::GamePadGetLeftX()
{
	return mGamePad.GetLeftX();
}

cfloat Input::GamePadGetLeftY()
{
	return mGamePad.GetLeftY();
}

cver2 Input::GamePadGetRight()
{
	return mGamePad.GetRight();
}

cfloat Input::GamePadGetRightX()
{
	return mGamePad.GetRightX();
}

cfloat Input::GamePadGetRightY()
{
	return mGamePad.GetRightY();
}

cint Input::GamePadGetLeftTrigger()
{
	return mGamePad.GetLeftTrigger();
}

cint Input::GamePadGetRightTrigger()
{
	return mGamePad.GetRightTrigger();
}

void Input::GamePadSetLeftVibration(crint speed)
{
	mGamePad.SetLeftVibration(speed);
}

void Input::GamePadSetRightVibration(crint speed)
{
	mGamePad.SetRightVibration(speed);
}

void Input::GamePadSetVibration(crint speed)
{
	mGamePad.SetVibration(speed);
}

void Input::GamePadStopVibration()
{
	mGamePad.StopVibration();
}

cbool Input::GamePadPress(const DWORD& button)
{
	return mGamePad.Press(button);
}

cbool Input::GamePadTrigger(const DWORD& button)
{
	return mGamePad.Trigger(button);
}

cbool Input::Connect()
{
	return mGamePad.GetConnect();
}

void Input::Keyboard::Init()
{
	memset(m_OldKeyState, 0, 256);
	memset(m_KeyState, 0, 256);
}

void Input::Keyboard::Update()
{
	memcpy(m_OldKeyState, m_KeyState, 256);
	GetKeyboardState(m_KeyState);
}

cbool Input::Keyboard::Press(const BYTE& KeyCode)const
{
	return (m_KeyState[KeyCode] & 0x80);
}

cbool Input::Keyboard::Trigger(const BYTE& KeyCode)const
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

cbool Input::Mouse::Init(HINSTANCE hInst)
{
	HRESULT hr;
	HWND hWindow = GetWindow();

	if (!mDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&mDInput, NULL);
	}

	bool result = false;
	// デバイス作成
	if (FAILED(mDInput->CreateDevice(GUID_SysMouse, &mMouse, NULL)) || mMouse == NULL)
	{
		MessageBox(hWindow, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// データフォーマット設定
	if (FAILED(mMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWindow, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// 他のアプリと協調モードに設定
	if (FAILED(mMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWindow, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// デバイスの設定
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// マウスの移動値　相対値

	if (FAILED(mMouse->SetProperty(DIPROP_AXISMODE, &prop.diph)))
	{
		MessageBox(hWindow, "Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	result = true;
	// アクセス権を得る
	mMouse->Acquire();
	return result;
}

void Input::Mouse::Update()
{
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = mMouseState;
	// データ取得
	if (SUCCEEDED(mMouse->GetDeviceState(sizeof(mMouseState), &mMouseState)))
	{
		mMouseTrigger.lX = mMouseState.lX;
		mMouseTrigger.lY = mMouseState.lY;
		mMouseTrigger.lZ = mMouseState.lZ;

		// マウスのボタン状態
		for (int i = 0; i < 8; i++)
		{
			mMouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mMouseState.rgbButtons[i]) & mMouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		mMouse->Acquire();
	}
}

void Input::Mouse::Uninit()
{
	if (mMouse)
	{
		mMouse->Unacquire();
		mMouse->Release();
		mMouse = NULL;
	}
}

cbool Input::Mouse::LeftPress()const
{
	return cbool(mMouseState.rgbButtons[0] & 0x80);
}

cbool Input::Mouse::LeftTrigger()const
{
	return cbool(mMouseTrigger.rgbButtons[0] & 0x80);
}

cbool Input::Mouse::RightPress()const
{
	return cbool(mMouseState.rgbButtons[1] & 0x80);
}

cbool Input::Mouse::RightTrigger()const
{
	return cbool(mMouseTrigger.rgbButtons[1] & 0x80);
}

cbool Input::Mouse::CenterPress()const
{
	return cbool(mMouseState.rgbButtons[2] & 0x80);
}

cbool Input::Mouse::CenterTrigger()const
{
	return cbool(mMouseTrigger.rgbButtons[2] & 0x80);
}

const long Input::Mouse::GetX()const
{
	return mMouseState.lX;
}

const long Input::Mouse::GetY()const
{
	return mMouseState.lY;
}

const long Input::Mouse::GetZ()const
{
	return mMouseState.lZ;
}

cver3 Input::Mouse::GetInputVector3() const
{
	return cver3(static_cast<float>(mMouseState.lX), static_cast<float>(mMouseState.lY), static_cast<float>(mMouseState.lZ));
}

void Input::GamePad::Init()
{

	ZeroMemory(&mState, sizeof(GamePadState));

	XInputEnable(true);

	mMagnitude = -1000.0f;

	mConnect = false;

}

void Input::GamePad::Update()
{

	DWORD id = 0;

	XInputSetState(id, &mState.vibration);

	mState.last = mState.state;

	mConnect = false;

	if (XInputGetState(id, &mState.state) == ERROR_DEVICE_NOT_CONNECTED)return;

	mConnect = true;

	//トリガー作成
	mState.trigger.Gamepad.wButtons = ((mState.last.Gamepad.wButtons ^ mState.state.Gamepad.wButtons)) & mState.state.Gamepad.wButtons;

	{

		//左スティック情報の作成
		ver2 left(
			static_cast<float>(mState.state.Gamepad.sThumbLX),
			static_cast<float>(mState.state.Gamepad.sThumbLY));

		float magnitude = sqrtf((left.x * left.x) + (left.y * left.y));

		if (magnitude > 32767)magnitude = 32767;

		magnitude -= mMagnitude;

		if (magnitude <= 0.0f) {
			mState.state.Gamepad.sThumbLX = 0;
			mState.state.Gamepad.sThumbLY = 0;
		}

		if (mState.state.Gamepad.sThumbLX >= 0)mLeft.x = static_cast<float>(mState.state.Gamepad.sThumbLX) / 32767;
		else mLeft.x = static_cast<float>(mState.state.Gamepad.sThumbLX) / 32768;

		if (mState.state.Gamepad.sThumbLY >= 0)mLeft.y = static_cast<float>(mState.state.Gamepad.sThumbLY) / 32767;
		else mLeft.y = static_cast<float>(mState.state.Gamepad.sThumbLY) / 32768;

	}

	{
		//右スティック情報の作成
		ver2 right(
			static_cast<float>(mState.state.Gamepad.sThumbRX),
			static_cast<float>(mState.state.Gamepad.sThumbRY));

		float magnitude = sqrtf((right.x * right.x) + (right.y * right.y));

		if (magnitude > 32767)magnitude = 32767;

		magnitude -= mMagnitude;

		if (magnitude <= 0.0f) {
			mState.state.Gamepad.sThumbRX = 0;
			mState.state.Gamepad.sThumbRY = 0;
		}

		if (mState.state.Gamepad.sThumbRX >= 0)mRight.x = static_cast<float>(mState.state.Gamepad.sThumbRX) / 32767;
		else mRight.x = static_cast<float>(mState.state.Gamepad.sThumbRX) / 32768;

		if (mState.state.Gamepad.sThumbRY >= 0)mRight.y = static_cast<float>(mState.state.Gamepad.sThumbRY) / 32767;
		else mRight.y = static_cast<float>(mState.state.Gamepad.sThumbRY) / 32768;

	}

}

void Input::GamePad::Uninit()
{

	ZeroMemory(&mState, sizeof(GamePadState));

	XInputSetState(0, &mState.vibration);

	XInputEnable(false);

}

cbool Input::GamePad::Connect()
{
	return XInputGetState(0, &mState.state) == ERROR_SUCCESS;
}

cver2 Input::GamePad::GetLeft() const
{
	return mLeft;
}

cfloat Input::GamePad::GetLeftX() const
{
	return mLeft.x;
}

cfloat Input::GamePad::GetLeftY() const
{
	return mLeft.y;
}

cver2 Input::GamePad::GetRight() const
{
	return mRight;
}

cfloat Input::GamePad::GetRightX() const
{
	return mRight.x;
}

cfloat Input::GamePad::GetRightY() const
{
	return mRight.y;
}

cint Input::GamePad::GetLeftTrigger() const
{
	return static_cast<int>(mState.state.Gamepad.bLeftTrigger);
}

cint Input::GamePad::GetRightTrigger() const
{
	return static_cast<int>(mState.state.Gamepad.bRightTrigger);
}

void Input::GamePad::SetLeftVibration(crint speed)
{
	mState.vibration.wLeftMotorSpeed = (((speed % 256) + 1) * 256) - 1;
}

void Input::GamePad::SetRightVibration(crint speed)
{
	mState.vibration.wRightMotorSpeed = (((speed % 256) + 1) * 256) - 1;
}

void Input::GamePad::SetVibration(crint speed)
{
	cint num = speed % 256;
	mState.vibration.wLeftMotorSpeed = ((num + 1) * 256) - 1;
	mState.vibration.wRightMotorSpeed = ((num + 1) * 256) - 1;
}

void Input::GamePad::StopVibration()
{
	mState.vibration.wLeftMotorSpeed = 0;
	mState.vibration.wRightMotorSpeed = 0;
}

cbool Input::GamePad::Press(const DWORD& button) const
{
	return (button & mState.state.Gamepad.wButtons);
}

cbool Input::GamePad::Trigger(const DWORD& button) const
{
	return (button & mState.trigger.Gamepad.wButtons);
}
