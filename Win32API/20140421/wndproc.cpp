#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static BYTE vKey = 0x00;
	static POINT ptMouse = {0,0};
	static RECT rc = {0,0,0,0};
	static int r = 15;
	static COLORREF color = RGB(0,0,255);
	static bool bRPlus = true;
	static bool bGPlus = true;
	static bool bBPlus = true;


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

		HBRUSH hBrush = ::CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		HPEN hPen = ::CreatePen(PS_SOLID, 1, color);
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

		if (vKey == VK_ADD)
		{
			r++;
			::InvalidateRect(hWnd, &rc, TRUE);
		}
		else if (vKey == VK_SUBTRACT)
		{
			r--;
			::InvalidateRect(hWnd, &rc, TRUE);
		}
		else if (vKey == 'R')
		{
			if (GetRValue(color) == 255)
			{
				bRPlus = false;
			}
			else if (GetRValue(color) == 0)
			{
				bRPlus = true;
			}

			if (bRPlus)
			{
				color = RGB(GetRValue(color)+1,
					GetGValue(color),
					GetBValue(color));
			}
			else
			{
				color = RGB(GetRValue(color)-1,
					GetGValue(color),
					GetBValue(color));
			}
			::InvalidateRect(hWnd, &rc, TRUE);
		}
		else if (vKey == 'G')
		{
			if (GetGValue(color) == 255)
			{
				bGPlus = false;
			}
			else if (GetGValue(color) == 0)
			{
				bGPlus = true;
			}

			if (bGPlus)
			{
				color = RGB(GetRValue(color),
					GetGValue(color)+1,
					GetBValue(color));
			}
			else
			{
				color = RGB(GetRValue(color),
					GetGValue(color)-1,
					GetBValue(color));
			}
			::InvalidateRect(hWnd, &rc, TRUE);
		}
		else if (vKey == 'B')
		{
			if (GetBValue(color) == 255)
			{
				bBPlus = false;
			}
			else if (GetBValue(color) == 0)
			{
				bBPlus = true;
			}

			if (bBPlus)
			{
				color = RGB(GetRValue(color),
					GetGValue(color),
					GetBValue(color)+1);
			}
			else
			{
				color = RGB(GetRValue(color),
					GetGValue(color),
					GetBValue(color)-1);
			}
			::InvalidateRect(hWnd, &rc, TRUE);
		}

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
