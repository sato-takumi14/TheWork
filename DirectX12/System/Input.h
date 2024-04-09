#pragma once

/***********************************************************************************************************
*
*	Input
*
*																				����ҁF������
*																				������F02/28
*
************************************************************************************************************
* ���͊֌W���܂Ƃ߂�
************************************************************************************************************/

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "Xinput.lib")

#include<windowsx.h>

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include<dinput.h>
#include<Xinput.h>

#include"System\Vector.h"

class Scene;

/*�Q�[���p�b�h�{�^��*/
namespace GamepadButton
{
	constexpr DWORD Up = 0x0001;			/*�\���L�[��*/
	constexpr DWORD Down = 0x0002;			/*�\���L�[��*/
	constexpr DWORD Left = 0x0004;			/*�\���L�[��*/
	constexpr DWORD Right = 0x0008;			/*�\���L�[�E*/
	constexpr DWORD Start = 0x0010;			/*�v���X�{�^��*/
	constexpr DWORD Back = 0x0020;			/*�}�C�i�X�{�^��*/
	constexpr DWORD LThumb = 0x0040;		/*���X�e�B�b�N��������*/
	constexpr DWORD RThumb = 0x0080;		/*�E�X�e�B�b�N��������*/
	constexpr DWORD L = 0x0100;				/*L�{�^��*/
	constexpr DWORD R = 0x0200;				/*R�{�^��*/
	constexpr DWORD ZL = 0x0400;			/*L2�{�^��*/
	constexpr DWORD ZR = 0x0800;			/*R2�{�^��*/
	constexpr DWORD A = 0x1000;				/*A�{�^��*/
	constexpr DWORD B = 0x2000;				/*B�{�^��*/
	constexpr DWORD X = 0x4000;				/*X�{�^��*/
	constexpr DWORD Y = 0x8000;				/*Y�{�^��*/
}

namespace Input
{

	namespace InputDevice
	{

		/*�L�[�{�[�h*/
		class Keyboard
		{
		private:

			BYTE mOldKeyState[256]{};				/*�O��̃X�e�[�g*/
			BYTE mKeyState[256]{};					/*���݂̃X�e�[�g*/

		public:

			/*�ʏ폈��*/
			void Init();							/*����������*/
			void Update();							/*�X�V����*/

			/*�L�[�{�[�h*/
			/*���������Ă���Ƃ�*/
			inline cbool Press(const BYTE& key)const		{ return mKeyState[key] & 0x80; }
			/*�������u��*/
			inline cbool Trigger(const BYTE& key)const		{ return (mKeyState[key] & 0x80) && not (mOldKeyState[key] & 0x80); }

		};

		/*�}�E�X*/
		class Mouse
		{
		private:

			LPDIRECTINPUT8			mDInput;			/*IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^*/
			LPDIRECTINPUTDEVICE8	mMouse;				/*mouse*/
			DIMOUSESTATE2			mMouseState;		/*�}�E�X�̃_�C���N�g�ȏ��*/
			DIMOUSESTATE2			mMouseTrigger;		/*�����ꂽ�u�Ԃ���ON*/

		public:

			/*�ʏ폈��*/
			cbool Init(const HINSTANCE& hInst);
			void Update();
			void Uninit();

			/*���N���b�N*/
			inline cbool LeftPress()const		{ return mMouseState.rgbButtons[0] & 0x80; }		/*���N���b�N������������*/
			inline cbool LeftTrigger()const		{ return mMouseTrigger.rgbButtons[0] & 0x80; }		/*���N���b�N�������u��*/

			/*�E�N���b�N*/
			inline cbool RightPress()const		{ return mMouseState.rgbButtons[1] & 0x80; }		/*�E�N���b�N������������*/
			inline cbool RightTrigger()const	{ return mMouseTrigger.rgbButtons[1] & 0x80; }		/*�E�N���b�N�������u��*/

			/*�}�E�X�z�C�[��*/
			inline cbool CenterPress()const		{ return mMouseState.rgbButtons[2] & 0x80; }		/*�}�E�X�z�C�[��������������*/
			inline cbool CenterTrigger()const	{ return mMouseTrigger.rgbButtons[2] & 0x80; }		/*�}�E�X�z�C�[���������u��*/

			/*�}�E�X�ړ���*/
			inline cfloat GetX()const { return static_cast<float>(mMouseState.lX); }				/*�����ړ���*/
			inline cfloat GetY()const { return static_cast<float>(mMouseState.lY); }				/*�c���ړ���*/
			inline cfloat GetZ()const { return static_cast<float>(mMouseState.lZ); }				/*�}�E�X�z�C�[���ړ���*/

			/*�ړ��ʂ̂��ׂ�*/
			inline cVector3 GetInput()const { return Vector3(static_cast<float>(mMouseState.lX), static_cast<float>(mMouseState.lY), static_cast<float>(mMouseState.lZ)); }

		};

		/*�Q�[���p�b�h*/
		class Gamepad
		{
		private:

			typedef struct GamepadState {

				XINPUT_STATE		last;
				XINPUT_STATE		state;
				XINPUT_STATE		trigger;
				XINPUT_VIBRATION	vibration;

			}GamepadState;

		private:

			GamepadState mState{};		/*���݂̃X�e�[�g*/
			Vector2 mLeft{};			/*���X�e�B�b�N*/
			Vector2 mRight{};			/*�E�X�e�B�b�N*/
			float mMagnitude{};			/*臒l*/
			bool mConnect{};			/*�ڑ����Ă��邩*/

		public:

			/*�ʏ폈��*/
			void Init();
			void Update();
			void Uninit();

			/*�ڑ����Ă��邩*/
			inline cbool Connect()								{ return XInputGetState(0, &mState.state) == ERROR_SUCCESS; }
			inline cbool GetConnect()const						{ return mConnect; }

			/*���X�e�B�b�N*/
			inline cVector2 GetLeft()const						{ return mLeft; }
			inline cfloat GetLeftX()const						{ return mLeft.x; }
			inline cfloat GetLeftY()const						{ return mLeft.y; }
			inline cbool GetLeftTrigger()const					{ return static_cast<bool>(mState.state.Gamepad.bLeftTrigger); }

			/*�E�X�e�B�b�N*/
			inline cVector2 GetRight()const						{ return mRight; }
			inline cfloat GetRightX()const						{ return mRight.x; }
			inline cfloat GetRightY()const						{ return mRight.y; }
			inline cbool GetRightTrigger()const					{ return static_cast<bool>(mState.state.Gamepad.bRightTrigger); }

			/*�U���̐ݒ�*/
			inline void SetLeftVibration(crint speed)			{ mState.vibration.wLeftMotorSpeed = (((speed % 256) + 1) * 256) - 1; }
			inline void SetRightVibration(crint speed)			{ mState.vibration.wRightMotorSpeed = (((speed % 256) + 1) * 256) - 1; }
			inline void SetVibration(crint speed) {
				SetLeftVibration(speed);
				SetRightVibration(speed);
			}
			inline void StopVibration() {
				mState.vibration.wLeftMotorSpeed = 0;
				mState.vibration.wRightMotorSpeed = 0;
			}

			/*�{�^��*/
			inline cbool Press(const DWORD& button)const		{ return button & mState.state.Gamepad.wButtons; }
			inline cbool Trigger(const DWORD& button)const		{ return button & mState.trigger.Gamepad.wButtons; }

		};

	}

	/*���͊֌W���܂Ƃ߂��N���X*/
	class Device
	{	
	private:

		/*�e�f�o�C�X*/
		static InputDevice::Keyboard mKeyboard;		/*�L�[�{�[�h*/
		static InputDevice::Mouse mMouse;			/*�}�E�X*/
		static InputDevice::Gamepad mGamepad;		/*�Q�[���p�b�h*/

	public:

		/*����������*/
		inline static void MasterInit(const HINSTANCE& hInst) {

			mKeyboard.Init();
			mMouse.Init(hInst);
			mGamepad.Init();

		}

		/*�X�V����*/
		inline static void MasterUpdate() {

			mKeyboard.Update();
			mMouse.Update();
			mGamepad.Update();

		}

		/*�I������*/
		inline static void MasterUninit() {

			mGamepad.Uninit();
			mMouse.Uninit();

		}

		/*�e�f�o�C�X�Q�b�g�֐�*/
		static Input::InputDevice::Keyboard* GetKeyboard()			{ return &mKeyboard; }
		static Input::InputDevice::Mouse* GetMouse()				{ return &mMouse; }
		static Input::InputDevice::Gamepad* GetGamepad()			{ return  &mGamepad; }

		/*�L�[�{�[�h*/
		static inline cbool KeyboardPress(const BYTE& key)			{ return mKeyboard.Press(key); }
		static inline cbool KeyboardTrigger(const BYTE& key)		{ return mKeyboard.Trigger(key); }

		/*�}�E�X*/
		static inline cbool MouseLeftPress(const BYTE& key)			{ return mMouse.LeftPress(); }
		static inline cbool MouseLeftTrigger(const BYTE& key)		{ return mMouse.LeftTrigger(); }
		static inline cbool MouseRightPress(const BYTE& key)		{ return mMouse.RightPress(); }
		static inline cbool MouseRightTrigger(const BYTE& key)		{ return mMouse.RightTrigger(); }
		static inline cbool MouseCenterPress(const BYTE& key)		{ return mMouse.CenterPress(); }
		static inline cbool MouseCenterTrigger(const BYTE& key)		{ return mMouse.CenterTrigger(); }
		static inline cfloat MouseX()								{ return mMouse.GetX(); }
		static inline cfloat MouseY()								{ return mMouse.GetY(); }
		static inline cfloat MouseZ()								{ return mMouse.GetZ(); }
		static inline cVector3 MouseVector3()						{ return mMouse.GetInput(); }

		/*�Q�[���p�b�h*/
		static inline cVector2 GamepadLeft()						{ return mGamepad.GetLeft(); }
		static inline cfloat GamepadLeftX()							{ return mGamepad.GetLeftX(); }
		static inline cfloat GamepadLeftY()							{ return mGamepad.GetLeftY(); }
		static inline cVector2 GamepadRight()						{ return mGamepad.GetRight(); }
		static inline cfloat GamepadRightX()						{ return mGamepad.GetRightX(); }
		static inline cfloat GamepadRightY()						{ return mGamepad.GetRightY(); }
		static inline cbool GamepadLeftTrigger()					{ return mGamepad.GetLeftTrigger(); }
		static inline cbool GamepadRightTrigger()					{ return mGamepad.GetRightTrigger(); }
		static inline void GamepadSetLeftVibration(crint speed)		{ mGamepad.SetLeftVibration(speed); }
		static inline void GamepadSetRightVibration(crint speed)	{ mGamepad.SetRightVibration(speed); }
		static inline void GamepadSetVibration(crint speed)			{ mGamepad.SetVibration(speed); }
		static inline void GamepadStopVibration()					{ mGamepad.StopVibration(); }
		static inline cbool GamepadPress(const DWORD& button)		{ return mGamepad.Press(button); }
		static inline cbool GamepadTrigger(const DWORD& button)		{ return mGamepad.Trigger(button); }
		static inline cbool Connect()								{ return mGamepad.Connect(); }

	};

	namespace ActionInfo
	{

		class Keyboard
		{
		public:

			std::function<cbool(const BYTE&)> input{};
			BYTE key{};
			string scene;

		public:

			inline Keyboard(
				std::function<cbool(const BYTE&)>& input,
				const BYTE& key,
				crstring scene) : 
				input(input), key(key), scene(scene) {}

		};

		class Gamepad
		{
		public:

			std::function<cbool(const DWORD&)> input{};
			DWORD key{};
			string scene{};

		public:

			Gamepad(
				std::function<cbool(const DWORD&)>& input,
				const DWORD& key,
				crstring scene) :
				input(input), key(key), scene(scene) {}

		};

	}

	class Action
	{
	private:

		static std::unordered_map<string, std::list<ActionInfo::Keyboard>> mKeyboard;
		static std::unordered_map<string, std::list<ActionInfo::Gamepad>> mGamepad;
		static Scene* mScene;

	public:

		/*�ʏ폈��*/
		static void MasterInit();
		static void MasterUninit();
		static void Uninit();

		/*���݂̃V�[�����Z�b�g*/
		inline static void SetScene(Scene* scene) { mScene = scene; }

		/*�A�N�V������ǉ�*/
		static void AddAction(
			crstring name,
			std::function<cbool(const BYTE&)> func,
			const BYTE& key);
		static void AddAction(
			crstring name,
			std::function<cbool(const DWORD&)> func,
			const DWORD& key);

		/*���O��t�����A�N�V����������Ă��邩*/
		static cbool InputAction(crstring name);

	};

};