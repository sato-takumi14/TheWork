
#include "Main.h"
#include "Input.h"
#include "Interface\Scene.h"
#include "System\Window.h"

#pragma warning(disable : 4995)

namespace Input
{

	namespace InputDevice
	{

		/*�L�[�{�[�h����������*/
		void Keyboard::Init()
		{
			memset(mOldKeyState, 0, 256);
			memset(mKeyState, 0, 256);
		}

		/*�L�[�{�[�h�X�V����*/
		void Keyboard::Update()
		{
			memcpy(mOldKeyState, mKeyState, 256);
			assert(GetKeyboardState(mKeyState));
		}

		/*�}�E�X����������*/
		cbool Mouse::Init(const HINSTANCE& hInst)
		{

			HRESULT hr;
			HWND hWindow = Window::Device::GetWindow();

			if (!mDInput)
			{
				// DirectInput�I�u�W�F�N�g�̍쐬
				hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
					IID_IDirectInput8, (void**)&mDInput, NULL);
			}

			bool result = false;
			// �f�o�C�X�쐬
			if (FAILED(mDInput->CreateDevice(GUID_SysMouse, &mMouse, NULL)) || mMouse == NULL)
			{
				MessageBox(hWindow, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
				return result;
			}
			// �f�[�^�t�H�[�}�b�g�ݒ�
			if (FAILED(mMouse->SetDataFormat(&c_dfDIMouse2)))
			{
				MessageBox(hWindow, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
				return result;
			}
			// ���̃A�v���Ƌ������[�h�ɐݒ�
			if (FAILED(mMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				MessageBox(hWindow, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
				return result;
			}

			// �f�o�C�X�̐ݒ�
			DIPROPDWORD prop;

			prop.diph.dwSize = sizeof(prop);
			prop.diph.dwHeaderSize = sizeof(prop.diph);
			prop.diph.dwObj = 0;
			prop.diph.dwHow = DIPH_DEVICE;
			prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

			if (FAILED(mMouse->SetProperty(DIPROP_AXISMODE, &prop.diph)))
			{
				MessageBox(hWindow, "Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
				return result;
			}

			result = true;
			// �A�N�Z�X���𓾂�
			mMouse->Acquire();
			return result;

		}

		/*�}�E�X�X�V����*/
		void Mouse::Update()
		{

			// �O��̒l�ۑ�
			DIMOUSESTATE2 lastMouseState = mMouseState;
			// �f�[�^�擾
			if (SUCCEEDED(mMouse->GetDeviceState(sizeof(mMouseState), &mMouseState)))
			{
				mMouseTrigger.lX = mMouseState.lX;
				mMouseTrigger.lY = mMouseState.lY;
				mMouseTrigger.lZ = mMouseState.lZ;

				// �}�E�X�̃{�^�����
				for (int i = 0; i < 8; i++)
				{
					mMouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
						mMouseState.rgbButtons[i]) & mMouseState.rgbButtons[i]);
				}
			}
			else	// �擾���s
			{
				// �A�N�Z�X���𓾂Ă݂�
				mMouse->Acquire();
			}

		}

		/*�}�E�X�I������*/
		void Mouse::Uninit()
		{

			if (mMouse)
			{
				mMouse->Unacquire();
				mMouse->Release();
				mMouse = NULL;
			}

		}

		/*�Q�[���p�b�h����������*/
		void Gamepad::Init()
		{

			ZeroMemory(&mState, sizeof(GamepadState));

			XInputEnable(true);

			mMagnitude = -1000.0f;

			mConnect = false;

		}

		/*�Q�[���p�b�h�X�V����*/
		void Gamepad::Update()
		{

			DWORD id = 0;

			XInputSetState(id, &mState.vibration);

			mState.last = mState.state;

			mConnect = false;

			if (XInputGetState(id, &mState.state) == ERROR_DEVICE_NOT_CONNECTED)return;

			mConnect = true;

			//�g���K�[�쐬
			mState.trigger.Gamepad.wButtons = ((mState.last.Gamepad.wButtons ^ mState.state.Gamepad.wButtons)) & mState.state.Gamepad.wButtons;

			{

				//���X�e�B�b�N���̍쐬
				Vector2 left(
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
				//�E�X�e�B�b�N���̍쐬
				Vector2 right(
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

		/*�Q�[���p�b�h�I������*/
		void Gamepad::Uninit()
		{

			ZeroMemory(&mState, sizeof(GamepadState));

			XInputSetState(0, &mState.vibration);

			XInputEnable(false);

		}
	}

	/*�f�o�C�X��static�ϐ�*/
	InputDevice::Keyboard	Input::Device::mKeyboard{};
	InputDevice::Mouse		Input::Device::mMouse{};
	InputDevice::Gamepad	Input::Device::mGamepad{};

	/*�A�N�V�����L�[��static�ϐ�*/
	std::unordered_map<string, std::list<ActionInfo::Keyboard>> Action::mKeyboard{};
	std::unordered_map<string, std::list<ActionInfo::Gamepad>> Action::mGamepad{};
	Scene* Action::mScene{};

	void Action::MasterInit()
	{
	}

	void Action::MasterUninit()
	{
		mKeyboard.clear();
		mGamepad.clear();
	}

	void Action::Uninit()
	{

		cstring scene = mScene->GetName();
		for (auto& key : mKeyboard) { key.second.remove_if([&](ActionInfo::Keyboard input) { return scene == input.scene; }); }
		for (auto& key : mGamepad) { key.second.remove_if([&](ActionInfo::Gamepad input) { return scene == input.scene; }); }

	}

	void Action::AddAction(
		crstring name, 
		std::function<cbool(const BYTE&)> func,
		const BYTE& key)
	{
		mKeyboard[name].push_back(ActionInfo::Keyboard(func, key, mScene->GetName()));
	}

	void Action::AddAction(
		crstring name, 
		std::function<cbool(const DWORD&)> func,
		const DWORD& key)
	{
		mGamepad[name].push_back(ActionInfo::Gamepad(func, key, mScene->GetName()));
	}

	cbool Action::InputAction(crstring name)
	{
		
		for (auto& data : mKeyboard[name]) { if (data.input(data.key))return true; }
		for (auto& data : mGamepad[name]) { if (data.input(data.key))return true; }

		return false;

	}

}
