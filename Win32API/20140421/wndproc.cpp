#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static BYTE vKey = 0x00;
	static POINT ptMouse = {0,0};
	static RECT rc = {0,0,0,0};

	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		// Handle Device Context
		HDC hdc = ::BeginPaint(hWnd, &ps);

		TCHAR szBuffer[100];

		_stprintf_s(szBuffer, _T("%x\r\nx = %d\r\ny = %d"), vKey, ptMouse.x, ptMouse.y);

		::DrawText(hdc, szBuffer, -1, &rc, DT_TOP | DT_LEFT);

		const int r = 15;

		HBRUSH hBrush = ::CreateSolidBrush(RGB(0,0,255));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(0,0,255));
		HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

		::Ellipse(hdc, ptMouse.x-r,ptMouse.y-r,
			ptMouse.x+r,ptMouse.y+r);

		::SelectObject(hdc, hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hdc, hOldBrush);
		::DeleteObject(hBrush);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_KEYDOWN)
	{
		vKey = wParam;

		::InvalidateRect(hWnd, &rc, TRUE);
		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		ptMouse.x = GET_X_LPARAM(lParam);
		ptMouse.y = GET_Y_LPARAM(lParam);

		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
