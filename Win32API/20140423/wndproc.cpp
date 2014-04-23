#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static POINT ptCircle = {200,100};
	static const LONG radius = 20;
	static LONG velocity = 0;

	if (uMsg == WM_CREATE)
	{
		::SetTimer(hWnd, 0, 10, NULL);
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		// Handle Device Context
		HDC hdc = ::BeginPaint(hWnd, &ps);

		::Ellipse(hdc, 
			ptCircle.x - radius,
			ptCircle.y - radius,
			ptCircle.x + radius,
			ptCircle.y + radius);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		velocity += 1;
		ptCircle.y += velocity;

		if (ptCircle.y + radius >= 400)
		{
			velocity = -velocity - 1;
		}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
