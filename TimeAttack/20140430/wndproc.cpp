#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	const int count = 20;
	static Data marble[count];
	static POINT spt;
	static POINT ept;
	static RECT box;
	static bool drag = false;
	static MouseBlock mb;
	static DWORD dt = 0;
	static DWORD st = ::GetTickCount();
	static HBITMAP hCursor;
	static RECT rcCursor = {3,3,42,43};
	static POINT ptMouse;

	////////////////////////////////////
	// double buffering
	static HDC hMainDC;
	static HDC hMemDC;
	static HBITMAP hMemBitmap;
	static HBITMAP hOldMemBitmap;

	if (uMsg == WM_CREATE)
	{
		hCursor = (HBITMAP)::LoadImage(NULL, _T("cursor.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

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

		mb.Attach(hWnd);

		::ShowCursor(FALSE);

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::ShowCursor(TRUE);

		::DeleteObject(hCursor);

		mb.Detach();

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

		::SetDCBrushColor(hMemDC, RGB(200,200,255));
		::FillRect(hMemDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

		for (int i = 0; i < count; i++)
		{
			marble[i].Draw(hMemDC);
		}

		if (drag)
		{
			mb.Draw(hMemDC);
		}

		HDC hBitmapDC = ::CreateCompatibleDC(hdc);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hCursor);

		// draw
		::GdiTransparentBlt(hMemDC, ptMouse.x - 20, ptMouse.y - 20, 40, 41,
			hBitmapDC, rcCursor.left, rcCursor.top, rcCursor.right-rcCursor.left,
			rcCursor.bottom-rcCursor.top, RGB(0,0,255));

		::SelectObject(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);

		::BitBlt(hdc, 0, 0, cx, cy, hMemDC, 0, 0, SRCCOPY);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		if (drag)
		{

			mb.SetEndPoint(ptMouse);

			for (int i = 0; i < count; i++)
			{
				if (::PtInRect(&mb.GetBox(), marble[i].center))
				{
					marble[i].select = true;
				}
				else
				{
					marble[i].select = false;
				}
			}

		}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

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

			mb.SetStartPoint(ptMouse);
			mb.SetEndPoint(ptMouse);
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
	else if (uMsg == WM_RBUTTONDOWN)
	{
		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		const int margin = 10;

		for (int i = 0; i < count; i++)
		{
			if (marble[i].select)
			{
				marble[i].destination.x = ptMouse.x - margin + rand()%(margin*2);
				marble[i].destination.y = ptMouse.y - margin + rand()%(margin*2);
			}
		}

		st = ::GetTickCount();
		::SetTimer(hWnd, 0, 10, NULL);

		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		bool ret = false;
		for (int i = 0; i < count; i++)
		{
			ret = (marble[i].Update(dt) || ret);
		}
		dt = ::GetTickCount() - st;
		st = ::GetTickCount();

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		if (!ret)
		{
			::KillTimer(hWnd, 0);
		}

		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}