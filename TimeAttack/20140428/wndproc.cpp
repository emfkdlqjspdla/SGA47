#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	const int count = 9;
	static RECT rc[count];
	static bool bDraw[count] = {false};

	if (uMsg == WM_CREATE)
	{
		RECT rcClient;
		::GetClientRect(hWnd, &rcClient);

		int width = (rcClient.right - rcClient.left)/3;
		int height = (rcClient.bottom - rcClient.top)/3;

		for (int i = 0; i < count; i++)
		{
			rc[i].left = 0 + width*(i%3);
			rc[i].right = width + width*(i%3);
			rc[i].top = 0 + height*(i/3);
			rc[i].bottom = height + height*(i/3);
		}

		for (int i = 0; i < count; i++)
		{
			bDraw[i] = false;
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
			::Rectangle(hdc, rc[i].left, rc[i].top, rc[i].right, rc[i].bottom);
			if (bDraw[i])
			{
				::Ellipse(hdc, rc[i].left, rc[i].top, rc[i].right, rc[i].bottom);
			}
		}

		::EndPaint(hWnd, &ps);

		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		for (int i = 0; i < count; i++)
		{
			if (rc[i].left < ptMouse.x && rc[i].right > ptMouse.x &&
				rc[i].top < ptMouse.y && rc[i].bottom > ptMouse.y)

			//if (::PtInRect(&rc[i], ptMouse))
			{
				if (bDraw[i])
					bDraw[i] = false;
				else
					bDraw[i] = true;
			}
		}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}