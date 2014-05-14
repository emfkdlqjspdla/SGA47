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

	int width = 400;
	int height = 400;

	int x = (cx - width)/2;
	int y = (cy - height)/2;

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	dwStyle = dwStyle & ~WS_THICKFRAME;
	//dwStyle = dwStyle & ~WS_MINIMIZEBOX;
	dwStyle = dwStyle & ~WS_MAXIMIZEBOX;

	HWND hWnd = ::CreateWindowEx(0, szClassName, _T("Win32 Game"), dwStyle,
		x, y, width, height,
		NULL, NULL, hInst, NULL);

	if (hWnd == NULL)
	{
		return 2;
	}

	::ShowWindow(hWnd, SW_NORMAL);
	::UpdateWindow(hWnd);

	Rect rc;
	::GetClientRect(hWnd, &rc);

	HDC hMainDC = ::GetDC(hWnd);
	HDC hMemDC = ::CreateCompatibleDC(hMainDC);
	HBITMAP hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rc.width(), rc.height());
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

	Tank a(rc.center()>>Size(0,rc.height()/2), 40);

	MSG msg;

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

		::SetDCBrushColor(hMemDC, RGB(255,255,255));
		::FillRect(hMemDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

		a.Input(dt);
		a.Update(dt);
		a.Draw(hMemDC);

		::BitBlt(hMainDC, 0, 0, rc.width(), rc.height(),
			hMemDC, 0, 0, SRCCOPY);

		dt = ::GetTickCount() - st;
		st = ::GetTickCount();
	}

	::SelectObject(hMemDC, hOldBitmap);
	::DeleteObject(hMemBitmap);
	::DeleteDC(hMemDC);
	::ReleaseDC(hWnd, hMainDC);

	return msg.wParam;
}