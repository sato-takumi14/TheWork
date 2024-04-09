#pragma once

/***********************************************************************************************************
*
*	Window
*
*																				制作者：佐藤匠
*																				制作日：03/17
*
************************************************************************************************************
* ウィンドウを管理するクラス
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