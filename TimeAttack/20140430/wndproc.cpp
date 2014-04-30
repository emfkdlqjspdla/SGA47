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
	static RECT box[4] = {{0,0,0,0}};
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
	else if (uMsg == WM_ERASEBKGND)
	{
		return 1;
	}
	else if (uMsg == WM_PAINT)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		HDC hMemDC = ::CreateCompatibleDC(hdc);
		HBITMAP hMemBitmap = ::CreateCompatibleBitmap(hdc, cx, cy);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

		::SetDCBrushColor(hMemDC, RGB(255,255,255));
		::FillRect(hMemDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

		for (int i = 0; i < count; i++)
		{
			marble[i].Draw(hMemDC);
		}

		if (drag)
		{
			POINT pt;
			::MoveToEx(hMemDC, box[0].left, box[0].top, &pt);
			::LineTo(hMemDC, box[0].left, box[0].bottom);
			::LineTo(hMemDC, box[0].right, box[0].bottom);
			::LineTo(hMemDC, box[0].right, box[0].top);
			::LineTo(hMemDC, box[0].left, box[0].top);
		}

		::BitBlt(hdc, 0, 0, cx, cy, hMemDC, 0, 0, SRCCOPY);

		::SelectObject(hMemDC, hOldBitmap);
		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);

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

			box[0].right = ptMouse.x;
			box[0].bottom = ptMouse.y;

			box[1].left = ptMouse.x;
			box[1].top = ptMouse.y;

			box[2].right = ptMouse.x;
			box[2].top = ptMouse.y;

			box[3].left = ptMouse.x;
			box[3].bottom = ptMouse.y;

			for (int i = 0; i < count; i++)
			{
				if (::PtInRect(&box[0], marble[i].center) ||
					::PtInRect(&box[1], marble[i].center) ||
					::PtInRect(&box[2], marble[i].center) ||
					::PtInRect(&box[3], marble[i].center))
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

			box[0].left = ptMouse.x;
			box[0].top = ptMouse.y;

			box[1].right = ptMouse.x;
			box[1].bottom = ptMouse.y;

			box[2].left = ptMouse.x;
			box[2].bottom = ptMouse.y;

			box[3].right = ptMouse.x;
			box[3].top = ptMouse.y;
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