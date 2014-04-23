#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		::SetTimer(hWnd, 0, 1000, NULL);
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);
		//::InvalidateRect(hWnd, &rc, TRUE);
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

		LONG size = 10;
		RECT box = {(rc.right-rc.left)/2 - size,
			(rc.bottom-rc.top)/2 - size,
			(rc.right-rc.left)/2 + size,
			(rc.bottom-rc.top)/2 + size};
		LONG width = box.right - box.left;
		LONG height = box.bottom - box.top;

		RECT rcDraw;

		BYTE r = rand()%256;
		BYTE g = rand()%256;
		BYTE b = rand()%256;

		COLORREF color = RGB(r,g,b);

		for (int y = 0, i = 0; y < rc.bottom - rc.top; y += height, i++)
		{
			r = min(GetRValue(color) + 10*i, 255);
			g = min(GetGValue(color) + 10*i, 255);
			b = min(GetBValue(color) + 10*i, 255);

			for (int x = 0; x < rc.right - rc.left; x += width)
			{
				prev = color;
				if (x != 0)
				{
					r = min(GetRValue(color) + 10, 255);
					g = min(GetGValue(color) + 10, 255);
					b = min(GetBValue(color) + 10, 255);
				}
				color = RGB(r,g,b);

				HBRUSH hRandBrush = ::CreateSolidBrush(color);

				rcDraw = box;
				::OffsetRect(&rcDraw, x, y);
				::FillRect(hdc, &rcDraw, hRandBrush);

				rcDraw = box;
				::OffsetRect(&rcDraw, -x, y);
				::FillRect(hdc, &rcDraw, hRandBrush);

				rcDraw = box;
				::OffsetRect(&rcDraw, x, -y);
				::FillRect(hdc, &rcDraw, hRandBrush);

				rcDraw = box;
				::OffsetRect(&rcDraw, -x, -y);
				::FillRect(hdc, &rcDraw, hRandBrush);

				::DeleteObject(hRandBrush);

				//hRandBrush = ::CreateSolidBrush(prev);

				//rcDraw = box;
				//::OffsetRect(&rcDraw, -x, y);
				//::FillRect(hdc, &rcDraw, hRandBrush);

				//rcDraw = box;
				//::OffsetRect(&rcDraw, -x, -y);
				//::FillRect(hdc, &rcDraw, hRandBrush);

				//::DeleteObject(hRandBrush);

			}
		}

		::EndPaint(hWnd, &ps);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
