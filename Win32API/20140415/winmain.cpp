#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

INT WINAPI _tWinMain(HINSTANCE hInst,
				   HINSTANCE hPrevInst,
				   LPTSTR lpCmdLine,
				   INT nShowCmd)
{
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

	HWND hWnd = ::CreateWindowEx(0, 
				szClassName, 
				_T("Win32 Sample"),
				WS_OVERLAPPEDWINDOW, 
				100,
				100,
				400,
				300,
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

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
