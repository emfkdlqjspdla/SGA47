#include "windefine.h"


INT WINAPI _tWinMain(HINSTANCE hInst,
				   HINSTANCE hPrevInst,
				   LPTSTR lpCmdLine,
				   INT nShowCmd)
{
	srand((unsigned)time(NULL));

	LPCTSTR szClassName = _T("MainWindowClass");
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_VREDRAW | CS_HREDRAW;
    wcex.lpfnWndProc = &WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szClassName;
    wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);

	if (!::RegisterClassEx(&wcex))
	{
		DWORD dwError = ::GetLastError();
		return 1;
	}

	int width = 400;
	int height = 300;

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	int x = (cx - width)/2;
	int y = (cy - height)/2;


	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	dwStyle = dwStyle & ~WS_MAXIMIZEBOX;

	HWND hWnd = ::CreateWindowEx(0, 
				szClassName, 
				_T("Win32 Sample"),
				dwStyle, 
				x,
				y,
				width,
				height,
				NULL,
				NULL,
				hInst,
				NULL);

	if (hWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		return 2;
	}

	::ShowWindow(hWnd, SW_NORMAL);
	::UpdateWindow(hWnd);

	MSG msg;

	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;
	}

	return msg.wParam;
}

