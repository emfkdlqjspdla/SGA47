﻿#include "windefine.h"
#include "MainGame.h"

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	LPCTSTR szClassName = _T("MyMainWindowClass");

	WNDCLASSEX wcex = {0};

    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_VREDRAW | CS_HREDRAW;
    wcex.lpfnWndProc = &WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = szClassName;
	wcex.hIconSm = wcex.hIcon;

	if (!::RegisterClassEx(&wcex))
	{
		return 1;
	}

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	int width = 500;
	int height = 400;

	int x = (cx - width)/2;
	int y = (cy - height)/2;

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	dwStyle = dwStyle & ~WS_THICKFRAME;
	//dwStyle = dwStyle & ~WS_MINIMIZEBOX;
	dwStyle = dwStyle & ~WS_MAXIMIZEBOX;

	HWND hWnd = ::CreateWindowEx(0, szClassName, _T("Win32 Mouse"), dwStyle,
		x, y, width, height,
		NULL, NULL, hInst, NULL);

	if (hWnd == NULL)
	{
		return 2;
	}

	::ShowWindow(hWnd, SW_NORMAL);
	::UpdateWindow(hWnd);

	RECT rc;
	::GetClientRect(hWnd, &rc);
	POINT lt = {rc.left, rc.top};
	POINT rb = {rc.right, rc.bottom};

	::ClientToScreen(hWnd, &lt);
	::ClientToScreen(hWnd, &rb);

	RECT rcScreen = {lt.x, lt.y, rb.x, rb.y};

	::ClipCursor(&rcScreen);


	MSG msg;

	MainGame mg;
	mg.Attach(hWnd);
	mg.Load();

	DWORD dt = 0;
	DWORD st = ::GetTickCount();

	while (true)
	{
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;

		mg.Input(dt);
		mg.Update(dt);
		mg.Draw();

		dt = ::GetTickCount() - st;
		st = ::GetTickCount();
	}
	return msg.wParam;
}