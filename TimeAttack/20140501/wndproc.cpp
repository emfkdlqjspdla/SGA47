#include "windefine.h"

struct Circle
{
	POINT center;
	LONG radius;
	COLORREF color;
	Circle(const LONG& r, COLORREF clr):radius(r), color(clr){}
	void SetCenter(const POINT& c)
	{
		center = c;
	}
	void Draw(HDC hdc)
	{
		HBRUSH hBrush = ::CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		HPEN hPen = ::CreatePen(PS_SOLID, 1, color);
		HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

		::Ellipse(hdc, center.x - radius, center.y - radius,
			center.x + radius, center.y + radius);

		::SelectObject(hdc, hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hdc, hOldBrush);
		::DeleteObject(hBrush);
	}
	POINT follow(const POINT& pt)
	{
		POINT tmp = center;

		center = pt;

		return tmp;
	}
};


LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	const int count = 15;
	static Circle* marble[count];
	static HDC hMainDC;
	static HDC hMemDC;
	static HBITMAP hMemBitmap, hOldBitmap;

	if (uMsg == WM_CREATE)
	{
		const int r[count] = {50,40,30,20,10};

		for (int i = 0; i < count; i++)
		{
			marble[i] = new Circle(50 - i*2, RGB(255, i*15, i*15));
		}
		RECT rc;
		::GetClientRect(hWnd, &rc);
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;

		hMainDC = ::GetDC(hWnd);

		hMemDC = ::CreateCompatibleDC(hMainDC);
		hMemBitmap = ::CreateCompatibleBitmap(hMainDC, cx, cy);
		hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

		::SetTimer(hWnd, 0, 10, NULL);
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		for (int i = 0; i < count; i++)
		{
			delete marble[i];
		}

		::SelectObject(hMemDC, hOldBitmap);
		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);
		::ReleaseDC(hWnd, hMainDC);

		::KillTimer(hWnd, 0);
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

		::SetDCBrushColor(hMemDC, RGB(255,255,255));
		::FillRect(hMemDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

		for (int i = 0; i < count; i++)
		{
			marble[count-i-1]->Draw(hMemDC);
		}

		::BitBlt(hdc, 0, 0, cx, cy, hMemDC, 0, 0, SRCCOPY);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		marble[0]->SetCenter(ptMouse);

		//POINT tmp = marble[0]->follow(ptMouse);
		//for (int i = 0; i < count-1; i++)
		//{
		//	tmp = marble[i+1]->follow(tmp);
		//}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		POINT tmp = marble[0]->follow(ptMouse);
		for (int i = 0; i < count-1; i++)
		{
			tmp = marble[i+1]->follow(tmp);
		}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}