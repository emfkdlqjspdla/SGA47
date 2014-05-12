#include "windefine.h"

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

	int width = 600;
	int height = 600;

	int x = (cx - width)/2;
	int y = (cy - height)/2;

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	dwStyle = dwStyle & ~WS_THICKFRAME;
	dwStyle = dwStyle & ~WS_MINIMIZEBOX;
	dwStyle = dwStyle & ~WS_MAXIMIZEBOX;

	HWND hWnd = ::CreateWindowEx(0, szClassName, _T("Win32 Shadow Game"), dwStyle,
		x, y, width, height,
		NULL, NULL, hInst, NULL);

	if (hWnd == NULL)
	{
		return 2;
	}

	::ShowWindow(hWnd, SW_NORMAL);
	::UpdateWindow(hWnd);


	MSG msg;

	while (true)
	{
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;

	}
	return msg.wParam;
}