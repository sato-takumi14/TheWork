
#include"Main.h"
#include"Manager\SceneManager.h"
#include"System\Input.h"
#include"System\Window.h"

//static cstring gClassName = "AppClass";
//static cstring gWindowName = "DirectX12";
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
//HWND gWindow;
//
//HWND GetWindow()
//{
//	return gWindow;
//}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	Window::Device device(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	return device.Update();

	/*WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = gClassName.c_str();
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);


		RECT rc = { 0, 0, (LONG)WindowWidth, (LONG)WindowHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		gWindow = CreateWindowEx(
			0, 
			gClassName.c_str(), 
			gWindowName.c_str(), 
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

	ShowWindow(gWindow, nCmdShow);
	UpdateWindow(gWindow);

	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	SceneManager::MasterInit();
	Input::Device::MasterInit(hInstance);

	MSG msg;
	while (true) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			if (msg.message == WM_QUIT)break;
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		}
		else {

			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)) {

				dwExecLastTime = dwCurrentTime;

				SceneManager::MasterUpdate();
				SceneManager::MasterDraw();

			}

		}

	}

	SceneManager::MasterUninit();

	timeEndPeriod(1);
	UnregisterClass(gClassName.c_str(), wcex.hInstance);

	return static_cast<int>(msg.wParam);*/

}

//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//
//	switch (uMsg) {
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//
//	case WM_KEYDOWN:
//		switch (wParam) {
//		case VK_ESCAPE:
//			DestroyWindow(hWnd);
//			break;
//		}
//		break;
//	}
//
//	return DefWindowProc(hWnd, uMsg, wParam, lParam);
//
//}
