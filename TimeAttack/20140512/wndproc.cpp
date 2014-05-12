#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static bool bShootingStar = false;
	static Star* StarDepot[100] = {NULL};
	static DWORD dt = 0;
	static DWORD st = 0;

	static DWORD update_dt = 0;
	static DWORD delay = 400;

	// double buffering
	static HDC hMainDC = NULL;
	static HDC hMemDC = NULL;
	static HBITMAP hMemBitmap = NULL;
	static HBITMAP hOldMemBitmap = NULL;

	if (uMsg == WM_CREATE)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		hMainDC = ::GetDC(hWnd);
		hMemDC = ::CreateCompatibleDC(hMainDC);
		hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rc.width(), rc.height());
		hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::SelectObject(hMemDC, hOldMemBitmap);
		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);
		::ReleaseDC(hWnd, hMainDC);

		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		bShootingStar = !bShootingStar;

		if (bShootingStar)
		{
			st = ::GetTickCount();

			::SetTimer(hWnd, 0, 10, NULL);
		}
		else
		{
			::KillTimer(hWnd, 0);
		}

		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		for (int i = 0; i < 100; i++)
		{
			if (StarDepot[i])
			{
				StarDepot[i]->Update(dt);
			}
		}

		if (update_dt > delay)
		{
			for (int i = 0; i < 100; i++)
			{
				if (StarDepot[i] == NULL)
				{
					StarDepot[i] = new Star(rand()%10 + 1);
					break;
				}
			}

			update_dt %= delay;
		}

		update_dt += dt;

		dt = ::GetTickCount() - st;
		st = ::GetTickCount();

		Rect rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		::SetDCBrushColor(hMemDC, RGB(0,0,0));
		::FillRect(hMemDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

		for (int i = 0; i < 100; i++)
		{
			if (StarDepot[i])
			{
				StarDepot[i]->Draw(hMemDC);
			}
		}

		// draw memory dc contents to real monitor.
		::BitBlt(hdc, 0, 0, rc.width(), rc.height(), hMemDC, 0, 0, SRCCOPY);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_ERASEBKGND)
	{
		return 1;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}