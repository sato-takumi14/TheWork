
#include"Main.h"
#include"Window.h"

#include"Manager\SceneManager.h"
#include"System\Input.h"

namespace Window
{

	HWND Device::mWindow{};

	LRESULT Device::WndProc(
		HWND hWnd, 
		UINT uMsg, 
		WPARAM wParam, 
		LPARAM lParam)
	{
		switch (uMsg) {

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			switch (wParam) {
			case VK_ESCAPE:
				DestroyWindow(hWnd);
				break;
			}
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	Device::Device(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
		/*ウィンドウ作成*/
		{
			mWcex.cbSize = sizeof(WNDCLASSEX);
			mWcex.style = 0;
			mWcex.lpfnWndProc = Window::Device::WndProc;
			mWcex.cbClsExtra = 0;
			mWcex.cbWndExtra = 0;
			mWcex.hInstance = hInstance;
			mWcex.hIcon = nullptr;
			mWcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			mWcex.hbrBackground = nullptr;
			mWcex.lpszMenuName = nullptr;
			mWcex.lpszClassName = ClassName;
			mWcex.hIconSm = nullptr;

			RegisterClassEx(&mWcex);

			RECT rc = { 0, 0, static_cast<long>(WindowWidth), static_cast<long>(WindowHeight) };
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

			mWindow = CreateWindowEx(
				0,
				ClassName,
				WindowName,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				rc.right - rc.left,
				rc.bottom - rc.top,
				nullptr,
				nullptr,
				hInstance,
				nullptr);

		}

		ShowWindow(mWindow, nCmdShow);
		UpdateWindow(mWindow);

		/*時間設定*/
		timeBeginPeriod(1);
		mDwExecLastTime = timeGetTime();
		mDwCurrentTime = 0;

		SceneManager::MasterInit();
		Input::Device::MasterInit(hInstance);

	}

	Device::~Device()
	{
		SceneManager::MasterUninit();
		timeEndPeriod(1);
		UnregisterClass(ClassName, mWcex.hInstance);
	}

	cint Device::Update()
	{
		MSG msg;
		while (true) {

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

				if (msg.message == WM_QUIT)break;
				else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

			}
			else {

				mDwCurrentTime = timeGetTime();

				if ((mDwCurrentTime - mDwExecLastTime) >= (1000 / 60)) {

					mDwExecLastTime = mDwCurrentTime;

					SceneManager::MasterUpdate();
					SceneManager::MasterDraw();

				}

			}

		}

		return static_cast<cint>(msg.wParam);

	}

}