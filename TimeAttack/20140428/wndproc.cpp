#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	const int count = 9;
	static RECT rc[count];
	static int state[count] = {0}; // 0 : blank, 1 : o, 2 : x
	static int nCheckCount = 0;
	static POINT spos, epos;

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
			state[i] = NONE;
		}

		spos.x = -1;
		spos.y = -1;
		epos.x = -1;
		epos.y = -1;

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		RECT rcClient;
		::GetClientRect(hWnd, &rcClient);
		int cx = rcClient.right - rcClient.left;
		int cy = rcClient.bottom - rcClient.top;

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		HDC hMemDC = ::CreateCompatibleDC(hdc);
		HBITMAP hMemBitmap = ::CreateCompatibleBitmap(hdc, cx, cy);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

		::SetDCBrushColor(hMemDC, RGB(255,255,255));
		::FillRect(hMemDC, &rcClient, (HBRUSH)::GetStockObject(DC_BRUSH));

		for (int i = 0; i < count; i++)
		{
			::Rectangle(hMemDC, rc[i].left, rc[i].top, rc[i].right, rc[i].bottom);
			if (state[i] == OOO)
			{
				check(hMemDC, rc[i], true);
			}
			else if (state[i] == XXX)
			{
				check(hMemDC, rc[i], false);
			}
		}

		HPEN hRedPen = ::CreatePen(PS_SOLID, 5, RGB(255,0,0));
		HPEN hOldPen = (HPEN)::SelectObject(hMemDC, hRedPen);

		POINT pt;
		::MoveToEx(hMemDC, spos.x, spos.y, &pt);
		::LineTo(hMemDC, epos.x, epos.y);

		::SelectObject(hMemDC, hOldPen);
		::DeleteObject(hRedPen);

		::BitBlt(hdc, 0, 0, cx, cy, hMemDC, 0, 0, SRCCOPY);

		::SelectObject(hMemDC, hOldBitmap);
		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);

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
			if (::PtInRect(&rc[i], ptMouse))
			{
				if (state[i] == NONE)
				{
					if (nCheckCount%2 == 0)
						state[i] = OOO;
					else
						state[i] = XXX;

					nCheckCount++;
				}
			}
		}

		// odello check
		// o check
		// horizon
		bool bCheck = true;
		for (int i = 0; i < 3; i++)
		{
			bCheck = true;
			for (int j = 0; j < 3; j++)
			{
				if (state[i*3 + j] != OOO)
				{
					bCheck = false;
					break;
				}
			}
			if (bCheck)
			{
				spos.x = rc[i].left;
				spos.y = (rc[i].bottom + rc[i].top)/2;
				epos.x = rc[i+2].right;
				epos.y = (rc[i].bottom + rc[i].top)/2;
			}
		}
		// vertical
		for (int i = 0; i < 3; i++)
		{
			bCheck = true;
			for (int j = 0; j < 3; j++)
			{
				if (state[j*3 + i] != OOO)
				{
					bCheck = false;
					break;
				}
			}
			if (bCheck)
			{
				spos.x = (rc[i].left + rc[i].right)/2;
				spos.y = rc[i].top;
				epos.x = (rc[i].left + rc[i].right)/2;
				epos.y = rc[i + 6].bottom;
			}
		}
		// 
		if (state[0] == OOO && state[4] == OOO && state[8] == OOO)
		{
			spos.x = rc[0].left;
			spos.y = rc[0].top;
			epos.x = rc[8].right;
			epos.y = rc[8].bottom;
		}
		// /
		if (state[2] == OOO && state[4] == OOO && state[6] == OOO)
		{
			spos.x = rc[2].right;
			spos.y = rc[2].top;
			epos.x = rc[6].left;
			epos.y = rc[6].bottom;
		}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void check(HDC hdc, const RECT& r, bool bCheck)
{
	int margin = 15;
	RECT tmp = r;
	::InflateRect(&tmp, -margin, -margin);
	// o
	if (bCheck)
	{
		::Ellipse(hdc, tmp.left, tmp.top, tmp.right, tmp.bottom);
	}
	// x
	else
	{
		POINT pt;
		::MoveToEx(hdc, tmp.left, tmp.top, &pt);
		::LineTo(hdc, tmp.right, tmp.bottom);
		::MoveToEx(hdc, tmp.right, tmp.top, &pt);
		::LineTo(hdc, tmp.left, tmp.bottom);
	}
}
