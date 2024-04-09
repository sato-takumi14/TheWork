#pragma once

/***********************************************************************************************************
*
*	Window
*
*																				����ҁF������
*																				������F03/17
*
************************************************************************************************************
* �E�B���h�E���Ǘ�����N���X
************************************************************************************************************/

#include"Main.h"

namespace Window
{

	constexpr const char* ClassName = "AppClass";
	constexpr const char* WindowName = "DirectX12";

	class Device
	{
	private:

		static HWND mWindow;
		DWORD mDwExecLastTime{};
		DWORD mDwCurrentTime{};
		WNDCLASSEX mWcex{};

	private:

		static LRESULT CALLBACK WndProc(
			HWND hWnd,
			UINT uMsg,
			WPARAM wParam,
			LPARAM lParam);

	public:

		inline static HWND GetWindow() { return mWindow; }

		Device(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
		~Device();

		cint Update();

	};

}