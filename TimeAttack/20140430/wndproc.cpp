#include "windefine.h"

struct Data
{
	RECT rc;
	bool select;
	const LONG radius;
	POINT center;
	Data()
		: radius(50)
	{
		rc.left = rc.top = rc.right = rc.bottom = 0;
		select = false;
	}
	void SetPosition(const POINT& pos)
	{
		center = pos;
		rc.left = pos.x - radius;
		rc.right = pos.x + radius;
		rc.top = pos.y - radius;
		rc.bottom = pos.y + radius;
	}
	void Draw(HDC hdc)
	{
		HBRUSH hBrush, hOldBrush;
		if (select)
		{
			hBrush = ::CreateSolidBrush(RGB(255,0,0));
			hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);
		}
		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
		if (select)
		{
			::SelectObject(hdc, hOldBrush);
			::DeleteObject(hBrush);
		}
	}
};

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	const int count = 20;
	static Data marble[count];
	static RECT box = {0,0,0,0};
	static bool drag = false;

	if (uMsg == WM_CREATE)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InflateRect(&rc, -50, -50);
		int minx = rc.left;
		int miny = rc.top;
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;
		for (int i = 0; i < 20; i++)
		{
			POINT pt = {minx + rand()%cx, miny + rand()%cy};
			marble[i].SetPosition(pt);
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
			marble[i].Draw(hdc);
		}

		if (drag)
		{
			POINT pt;
			::MoveToEx(hdc, box.left, box.top, &pt);
			::LineTo(hdc, box.left, box.bottom);
			::LineTo(hdc, box.right, box.bottom);
			::LineTo(hdc, box.right, box.top);
			::LineTo(hdc, box.left, box.top);
		}

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		if (drag)
		{
			POINT ptMouse;
			::GetCursorPos(&ptMouse);
			::ScreenToClient(hWnd, &ptMouse);

			box.right = ptMouse.x;
			box.bottom = ptMouse.y;


			for (int i = 0; i < count; i++)
			{
				if (::PtInRect(&box, marble[i].center))
				{
					marble[i].select = true;
				}
				else
				{
					marble[i].select = false;
				}
			}

			RECT rc;
			::GetClientRect(hWnd, &rc);
			::InvalidateRect(hWnd, &rc, TRUE);
		}

		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		if (drag == false)
		{
			for (int i = 0; i < count; i++)
			{
				marble[i].select = false;
			}

			POINT ptMouse;
			::GetCursorPos(&ptMouse);
			::ScreenToClient(hWnd, &ptMouse);

			box.left = ptMouse.x;
			box.top = ptMouse.y;
		}

		drag = true;

		return 0;
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		drag = false;

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}