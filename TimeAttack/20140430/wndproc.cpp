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
		HPEN hPen, hOldPen;
		if (select)
		{
			hBrush = ::CreateSolidBrush(RGB(255,100,100));
			hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);
			hPen = ::CreatePen(PS_SOLID, 1, RGB(255,0,0));
			hOldPen = (HPEN)::SelectObject(hdc, hPen);
		}
		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
		if (select)
		{
			::SelectObject(hdc, hOldPen);
			::DeleteObject(hPen);

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
	static HDC hMainDC;

	////////////////////////////////////
	// double buffering
	static HDC hMemDC;
	static HBITMAP hMemBitmap;
	static HBITMAP hOldMemBitmap;

	////////////////////////////////////
	// transparent
	static HDC hTransSrcDC;
	static HBITMAP hTransBitmap;
	static HBITMAP hOldTransBitmap;

	////////////////////////////////////
	// alpha
	static HDC hAlphaSrcDC;
	static HBITMAP hAlphaBitmap;
	static HBITMAP hOldAlphaBitmap;

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

		hMainDC = ::GetDC(hWnd);
		hMemDC = ::CreateCompatibleDC(hMainDC);
		hMemBitmap = ::CreateCompatibleBitmap(hMainDC, cx + 100, cy + 100);
		hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

		hTransSrcDC = ::CreateCompatibleDC(hMainDC);
		hTransBitmap = ::CreateCompatibleBitmap(hMainDC, cx + 100, cy + 100);
		hOldTransBitmap = (HBITMAP)::SelectObject(hTransSrcDC, hTransBitmap);

		hAlphaSrcDC = ::CreateCompatibleDC(hMainDC);
		hAlphaBitmap = ::CreateCompatibleBitmap(hMainDC, cx + 100, cy + 100);
		hOldAlphaBitmap = (HBITMAP)::SelectObject(hAlphaSrcDC, hAlphaBitmap);

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::SelectObject(hAlphaSrcDC, hOldAlphaBitmap);
		::DeleteObject(hAlphaBitmap);
		::DeleteDC(hAlphaSrcDC);

		::SelectObject(hTransSrcDC, hOldTransBitmap);
		::DeleteObject(hTransBitmap);
		::DeleteDC(hTransSrcDC);

		::SelectObject(hMemDC, hOldMemBitmap);
		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);
		::ReleaseDC(hWnd, hMainDC);

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
			marble[i].Draw(hMemDC);
		}

		if (drag)
		{
			::SetDCBrushColor(hTransSrcDC, RGB(255,255,255));
			::FillRect(hTransSrcDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

			HBRUSH hBrush = ::CreateSolidBrush(RGB(100,149,237));
			HBRUSH hOldBrush = (HBRUSH)::SelectObject(hTransSrcDC, hBrush);

			HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0,255,255));
			HPEN hOldPen = (HPEN)::SelectObject(hTransSrcDC, hPen);

			::Rectangle(hTransSrcDC, box[0].left, box[0].top, box[0].right, box[0].bottom);

			::SelectObject(hTransSrcDC, hOldPen);
			::DeleteObject(hPen);

			::SelectObject(hTransSrcDC, hOldBrush);
			::DeleteObject(hBrush);

			::GdiTransparentBlt(hAlphaSrcDC, 0, 0, cx, cy,
				hTransSrcDC, 0, 0, cx, cy, RGB(255,255,255));

			BLENDFUNCTION bf;
			bf.AlphaFormat = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.BlendOp = 0;
			bf.SourceConstantAlpha = 127;

			::GdiAlphaBlend(hMemDC, 
				box[0].left, box[0].top, 
				box[0].right - box[0].left, 
				box[0].bottom - box[0].top,
				hAlphaSrcDC, 
				box[0].left, box[0].top, 
				box[0].right - box[0].left, 
				box[0].bottom - box[0].top, bf);

		}

		::BitBlt(hdc, 0, 0, cx, cy, hMemDC, 0, 0, SRCCOPY);

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