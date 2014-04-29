#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	const int count = 9;
	static RECT rc[count];
	static COLORREF color[count];

	if (uMsg == WM_CREATE)
	{
		RECT rcClient;
		::GetClientRect(hWnd, &rcClient);
		int cx = (rcClient.right - rcClient.left)/3;
		int cy = (rcClient.bottom - rcClient.top)/3;

		for (int i = 0; i < count; i++)
		{
			rc[i].left = 0 + cx*(i%3);
			rc[i].right = rc[i].left + cx;
			rc[i].top = 0 + cy*(i/3);
			rc[i].bottom = rc[i].top + cy;
		}
		for (int i = 0; i < count; i++)
		{
			color[i] = RGB(255,0,0);
		}

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		for (int i = 0; i < count; i++)
		{
			HBRUSH hBrush = ::CreateSolidBrush(color[i]);
			HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

			::Rectangle(hdc, rc[i].left, rc[i].top, rc[i].right, rc[i].bottom);

			::SelectObject(hdc, hOldBrush);
			::DeleteObject(hBrush);
		}

		::EndPaint(hWnd, &ps);

		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		for (int i = 0; i < count; i++)
		{
			if (::PtInRect(&rc[i], ptMouse))
			{
				BYTE g = min(GetGValue(color[i]) + 1, 255);
				BYTE b = min(GetBValue(color[i]) + 1, 255);
				color[i] = RGB(255, g, b);
			}
		}

		RECT rcClient;
		::GetClientRect(hWnd, &rcClient);
		::InvalidateRect(hWnd, &rcClient, TRUE);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}