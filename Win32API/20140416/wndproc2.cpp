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

		// GDI object
		// pen, brush, font, path, region, pallete
		RECT rc;
		::GetClientRect(hWnd, &rc);

		RECT box = {0,0,50,50};
		LONG width = box.right - box.left;
		LONG height = box.bottom - box.top;

		RECT rcDraw;

		for (int y = 0; y < rc.bottom - rc.top; y += height)
		{
			for (int x = 0; x < rc.right - rc.left; x += width)
			{
				rcDraw = box;
				::OffsetRect(&rcDraw, x, y);

				BYTE r = rand()%256;
				BYTE g = rand()%256;
				BYTE b = rand()%256;
				HBRUSH hRandBrush = ::CreateSolidBrush(RGB(r,g,b));

				::FillRect(hdc, &rcDraw, hRandBrush);
				::DeleteObject(hRandBrush);

			}
		}

		::EndPaint(hWnd, &ps);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
