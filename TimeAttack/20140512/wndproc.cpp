#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static std::list<Star*> StarDepot;
	static DWORD dt = 0;
	static DWORD st = 0;

	static DWORD update_dt = 0;
	static DWORD delay = 10;

	// double buffering
	static HDC hMainDC = NULL;
	static HDC hMemDC = NULL;
	static HBITMAP hMemBitmap = NULL;
	static HBITMAP hOldMemBitmap = NULL;

	static Point ptMouse;

	// Critical Section
	static CRITICAL_SECTION cs;

	if (uMsg == WM_CREATE)
	{
		::InitializeCriticalSection(&cs);

		Rect rc;
		::GetClientRect(hWnd, &rc);

		hMainDC = ::GetDC(hWnd);
		hMemDC = ::CreateCompatibleDC(hMainDC);
		hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rc.width(), rc.height());
		hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

		dt = 0;
		st = ::GetTickCount();
		::SetTimer(hWnd, 0, 10, NULL);

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::KillTimer(hWnd, 0);

		::SelectObject(hMemDC, hOldMemBitmap);
		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);
		::ReleaseDC(hWnd, hMainDC);

		::DeleteCriticalSection(&cs);

		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		::GetCursorPos(&ptMouse);

		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		if (update_dt > delay)
		{
			int count = update_dt/delay;

			for (int i = 0; i < count; i++)
			{
				std::list<Star*>::iterator it;
				for (it = StarDepot.begin(); it != StarDepot.end(); it++)
				{
					(*it)->Update(dt);
				}

				Star* pStar = new Star(ptMouse.ToClient(hWnd), rand()%10 + 5, rand()%360);

				//::EnterCriticalSection(&cs);
				StarDepot.push_back(pStar);
				//::LeaveCriticalSection(&cs);

				for (it = StarDepot.begin(); it != StarDepot.end();)
				{
					if (!::PtInRect(&rc, (*it)->GetPosition()))
					{
						//::EnterCriticalSection(&cs);
						delete *it;
						it = StarDepot.erase(it);
						//::LeaveCriticalSection(&cs);
					}
					else
					{
						it++;
					}
				}
			}

			update_dt %= delay;
		}

		update_dt += dt;

		dt = ::GetTickCount() - st;
		st = ::GetTickCount();

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

		std::list<Star*>::iterator it;
		for (it = StarDepot.begin(); it != StarDepot.end(); it++)
		{
			(*it)->Draw(hMemDC);
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