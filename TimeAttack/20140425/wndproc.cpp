#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static int mode = 0;
	static POINT ptMouse = {0,0};
	const int size = 20;

	if (uMsg == WM_CREATE)
	{
		::ShowCursor(FALSE);
	}
	else if (uMsg == WM_DESTROY)
	{
		::ShowCursor(TRUE);
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);


		if (mode == 0)
		{
			POINT pt;
			::MoveToEx(hdc, ptMouse.x - size/2, ptMouse.y, &pt);
			::LineTo(hdc, ptMouse.x + size/2, ptMouse.y);
			::MoveToEx(hdc, ptMouse.x, ptMouse.y - size/2, &pt);
			::LineTo(hdc, ptMouse.x, ptMouse.y + size/2);
		}
		else if (mode == 1)
		{
			HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(255,0,0));
			HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

			HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,0));
			HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

			::Ellipse(hdc, ptMouse.x-size/2, ptMouse.y-size/2,
				ptMouse.x+size/2, ptMouse.y+size/2);

			::SelectObject(hdc, hOldPen);
			::DeleteObject(hPen);
			::SelectObject(hdc, hOldBrush);
			::DeleteObject(hBrush);
		}
		else if (mode == 2)
		{
			HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(0,0,255));
			HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

			HBRUSH hBrush = ::CreateSolidBrush(RGB(0,0,255));
			HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

			::Rectangle(hdc, ptMouse.x-size/2, ptMouse.y-size/2,
				ptMouse.x+size/2, ptMouse.y+size/2);

			::SelectObject(hdc, hOldPen);
			::DeleteObject(hPen);
			::SelectObject(hdc, hOldBrush);
			::DeleteObject(hBrush);

		}


		::EndPaint(hWnd, &ps);

		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		//ptMouse.x = GET_X_LPARAM(lParam);
		//ptMouse.y = GET_Y_LPARAM(lParam);
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
	}
	else if (uMsg == WM_KEYDOWN)
	{
		if (wParam == '1')
		{
			mode = 0;
		}
		else if (wParam == '2')
		{
			mode = 1;
		}
		else if (wParam == '3')
		{
			mode = 2;
		}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}