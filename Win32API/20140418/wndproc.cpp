#include "windefine.h"

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

		HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(255,0,0));
		HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

		POINT pt;

		::MoveToEx(hdc, 0, 0, &pt);
		::LineTo(hdc, 100, 100);

		::SelectObject(hdc, hOldPen);
		::DeleteObject(hPen);

		::EndPaint(hWnd, &ps);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
