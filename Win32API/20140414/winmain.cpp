#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

INT WINAPI wWinMain(HINSTANCE hInst,
				   HINSTANCE hPrevInst,
				   LPWSTR lpCmdLine,
				   INT nShowCmd)
{
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
    wcex.lpszClassName = L"MainWindowClass";
    wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);

	if (!::RegisterClassEx(&wcex))
	{
		DWORD dwError = ::GetLastError();
		return 1;
	}

	HWND hWnd = ::CreateWindowEx(0, 
				L"MainWindowClass", 
				L"Win32 Sample",
				WS_OVERLAPPEDWINDOW, 
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				NULL,
				NULL,
				hInst,
				NULL);

	if (hWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		return 2;
	}

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);

	MSG msg;

	while (::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
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
