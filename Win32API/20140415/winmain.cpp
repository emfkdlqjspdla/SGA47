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

	int width = 400;
	int height = 300;

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	int x = (cx - width)/2;
	int y = (cy - height)/2;

	HWND hWnd = ::CreateWindowEx(0, 
				szClassName, 
				_T("Win32 Sample"),
				WS_OVERLAPPEDWINDOW, 
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

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		// Handle Device Context
		HDC hdc = ::BeginPaint(hWnd, &ps);

		// GDI object
		// pen, brush, font, path, region, pallete
		RECT rc;
		::GetClientRect(hWnd, &rc);

		HBRUSH hBrush = ::CreateSolidBrush(RGB(0x00,0xbf,0xff));

		::FillRect(hdc, &rc, hBrush);

		::SetBkMode(hdc, TRANSPARENT);

		::DrawText(hdc, _T("Hello Win32 Programming"), -1, 
			&rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		::DeleteObject(hBrush);

		::EndPaint(hWnd, &ps);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
