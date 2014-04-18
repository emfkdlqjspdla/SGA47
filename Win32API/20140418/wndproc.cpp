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

		HBRUSH hBrush = ::CreateSolidBrush(RGB(0,0,255));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		POINT pt;

		::MoveToEx(hdc, 100, 100, &pt);
		::LineTo(hdc, 100, 200);
		::LineTo(hdc, 200, 200);
		::LineTo(hdc, 200, 100);
		::LineTo(hdc, 100, 100);

		::Rectangle(hdc, 150, 150, 250, 250);

		::Ellipse(hdc, 0,0,100,100);

		::SelectObject(hdc, hOldBrush);
		::DeleteObject(hBrush);

		::SelectObject(hdc, hOldPen);
		::DeleteObject(hPen);

		::EndPaint(hWnd, &ps);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
