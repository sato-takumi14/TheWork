#pragma once

/***********************************************************************************************************
*
*	Input
*
*																				制作者：佐藤匠
*																				制作日：02/28
*
************************************************************************************************************
* 入力関係をまとめた
************************************************************************************************************/

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "Xinput.lib")

#include<windowsx.h>

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include<dinput.h>
#include<Xinput.h>

#include"System\Vector.h"

class Scene;

/*ゲームパッドボタン*/
namespace GamepadButton
{
	constexpr DWORD Up = 0x0001;			/*十字キー上*/
	constexpr DWORD Down = 0x0002;			/*十字キー下*/
	constexpr DWORD Left = 0x0004;			/*十字キー左*/
	constexpr DWORD Right = 0x0008;			/*十字キー右*/
	constexpr DWORD Start = 0x0010;			/*プラスボタン*/
	constexpr DWORD Back = 0x0020;			/*マイナスボタン*/
	constexpr DWORD LThumb = 0x0040;		/*左スティック押し込み*/
	constexpr DWORD RThumb = 0x0080;		/*右スティック押し込み*/
	constexpr DWORD L = 0x0100;				/*Lボタン*/
	constexpr DWORD R = 0x0200;				/*Rボタン*/
	constexpr DWORD ZL = 0x0400;			/*L2ボタン*/
	constexpr DWORD ZR = 0x0800;			/*R2ボタン*/
	constexpr DWORD A = 0x1000;				/*Aボタン*/
	constexpr DWORD B = 0x2000;				/*Bボタン*/
	constexpr DWORD X = 0x4000;				/*Xボタン*/
	constexpr DWORD Y = 0x8000;				/*Yボタン*/
}

namespace Input
{

	namespace InputDevice
	{

		/*キーボード*/
		class Keyboard
		{
		private:

			BYTE mOldKeyState[256]{};				/*前回のステート*/
			BYTE mKeyState[256]{};					/*現在のステート*/

		public:

			/*通常処理*/
			void Init();							/*初期化処理*/
			void Update();							/*更新処理*/

			/*キーボード*/
			/*押し続けているとき*/
			inline cbool Press(const BYTE& key)const		{ return mKeyState[key] & 0x80; }
			/*押した瞬間*/
			inline cbool Trigger(const BYTE& key)const		{ return (mKeyState[key] & 0x80) && not (mOldKeyState[key] & 0x80); }

		};

		/*マウス*/
		class Mouse
		{
		private:

			LPDIRECTINPUT8			mDInput;			/*IDirectInput8インターフェースへのポインタ*/
			LPDIRECTINPUTDEVICE8	mMouse;				/*mouse*/
			DIMOUSESTATE2			mMouseState;		/*マウスのダイレクトな状態*/
			DIMOUSESTATE2			mMouseTrigger;		/*押された瞬間だけON*/

		public:

			/*通常処理*/
			cbool Init(const HINSTANCE& hInst);
			void Update();
			void Uninit();

			/*左クリック*/
			inline cbool LeftPress()const		{ return mMouseState.rgbButtons[0] & 0x80; }		/*左クリック押し続けた時*/
			inline cbool LeftTrigger()const		{ return mMouseTrigger.rgbButtons[0] & 0x80; }		/*左クリック押した瞬間*/

			/*右クリック*/
			inline cbool RightPress()const		{ return mMouseState.rgbButtons[1] & 0x80; }		/*右クリック押し続けた時*/
			inline cbool RightTrigger()const	{ return mMouseTrigger.rgbButtons[1] & 0x80; }		/*右クリック押した瞬間*/

			/*マウスホイール*/
			inline cbool CenterPress()const		{ return mMouseState.rgbButtons[2] & 0x80; }		/*マウスホイール押し続けた時*/
			inline cbool CenterTrigger()const	{ return mMouseTrigger.rgbButtons[2] & 0x80; }		/*マウスホイール押した瞬間*/

			/*マウス移動量*/
			inline cfloat GetX()const { return static_cast<float>(mMouseState.lX); }				/*横軸移動量*/
			inline cfloat GetY()const { return static_cast<float>(mMouseState.lY); }				/*縦軸移動量*/
			inline cfloat GetZ()const { return static_cast<float>(mMouseState.lZ); }				/*マウスホイール移動量*/

			/*移動量のすべて*/
			inline cVector3 GetInput()const { return Vector3(static_cast<float>(mMouseState.lX), static_cast<float>(mMouseState.lY), static_cast<float>(mMouseState.lZ)); }

		};

		/*ゲームパッド*/
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

			GamepadState mState{};		/*現在のステート*/
			Vector2 mLeft{};			/*左スティック*/
			Vector2 mRight{};			/*右スティック*/
			float mMagnitude{};			/*閾値*/
			bool mConnect{};			/*接続しているか*/

		public:

			/*通常処理*/
			void Init();
			void Update();
			void Uninit();

			/*接続しているか*/
			inline cbool Connect()								{ return XInputGetState(0, &mState.state) == ERROR_SUCCESS; }
			inline cbool GetConnect()const						{ return mConnect; }

			/*左スティック*/
			inline cVector2 GetLeft()const						{ return mLeft; }
			inline cfloat GetLeftX()const						{ return mLeft.x; }
			inline cfloat GetLeftY()const						{ return mLeft.y; }
			inline cbool GetLeftTrigger()const					{ return static_cast<bool>(mState.state.Gamepad.bLeftTrigger); }

			/*右スティック*/
			inline cVector2 GetRight()const						{ return mRight; }
			inline cfloat GetRightX()const						{ return mRight.x; }
			inline cfloat GetRightY()const						{ return mRight.y; }
			inline cbool GetRightTrigger()const					{ return static_cast<bool>(mState.state.Gamepad.bRightTrigger); }

			/*振動の設定*/
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

			/*ボタン*/
			inline cbool Press(const DWORD& button)const		{ return button & mState.state.Gamepad.wButtons; }
			inline cbool Trigger(const DWORD& button)const		{ return button & mState.trigger.Gamepad.wButtons; }

		};

	}

	/*入力関係をまとめたクラス*/
	class Device
	{	
	private:

		/*各デバイス*/
		static InputDevice::Keyboard mKeyboard;		/*キーボード*/
		static InputDevice::Mouse mMouse;			/*マウス*/
		static InputDevice::Gamepad mGamepad;		/*ゲームパッド*/

	public:

		/*初期化処理*/
		inline static void MasterInit(const HINSTANCE& hInst) {

			mKeyboard.Init();
			mMouse.Init(hInst);
			mGamepad.Init();

		}

		/*更新処理*/
		inline static void MasterUpdate() {

			mKeyboard.Update();
			mMouse.Update();
			mGamepad.Update();

		}

		/*終了処理*/
		inline static void MasterUninit() {

			mGamepad.Uninit();
			mMouse.Uninit();

		}

		/*各デバイスゲット関数*/
		static Input::InputDevice::Keyboard* GetKeyboard()			{ return &mKeyboard; }
		static Input::InputDevice::Mouse* GetMouse()				{ return &mMouse; }
		static Input::InputDevice::Gamepad* GetGamepad()			{ return  &mGamepad; }

		/*キーボード*/
		static inline cbool KeyboardPress(const BYTE& key)			{ return mKeyboard.Press(key); }
		static inline cbool KeyboardTrigger(const BYTE& key)		{ return mKeyboard.Trigger(key); }

		/*マウス*/
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

		/*ゲームパッド*/
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

		/*通常処理*/
		static void MasterInit();
		static void MasterUninit();
		static void Uninit();

		/*現在のシーンをセット*/
		inline static void SetScene(Scene* scene) { mScene = scene; }

		/*アクションを追加*/
		static void AddAction(
			crstring name,
			std::function<cbool(const BYTE&)> func,
			const BYTE& key);
		static void AddAction(
			crstring name,
			std::function<cbool(const DWORD&)> func,
			const DWORD& key);

		/*名前を付けたアクションがされているか*/
		static cbool InputAction(crstring name);

	};

};