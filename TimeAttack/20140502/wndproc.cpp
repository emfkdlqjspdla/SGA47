#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	const int count = 20;
	static Circle marble[count];
	static RECT box;
	static POINT ept;
	static POINT spt;
	static bool drag = false;
	static HBITMAP hCursor;
	static RECT rcCursor = {3,3,42,43};
	static POINT ptMouse;

	/////////////////////////////
	// double buffering
	static HDC hMainDC = NULL;
	static HDC hMemDC = NULL;
	static HBITMAP hMemBitmap = NULL;
	static HBITMAP hOldMemBitmap = NULL;

	if (uMsg == WM_CREATE)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);

		hMainDC = ::GetDC(hWnd);
		hMemDC = ::CreateCompatibleDC(hMainDC);
		hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rc.right-rc.left, rc.bottom - rc.top);
		hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

		hCursor = (HBITMAP)::LoadImage(NULL, _T("cursor.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);


		::InflateRect(&rc, -20, -20);
		int minx = rc.left;
		int miny = rc.top;
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;

		for (int i = 0; i < count ; i++)
		{
			marble[i].id = i;
			marble[i].center.x = minx + rand()%cx;
			marble[i].center.y = miny + rand()%cy;
			marble[i].select = false;
		}

		::ShowCursor(FALSE);

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::ShowCursor(TRUE);

		::DeleteObject(hCursor);

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

		RECT info = {rc.right - 60, rc.bottom - 140, rc.right, rc.bottom};

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		::SetDCBrushColor(hMemDC, RGB(255,255,255));
		::FillRect(hMemDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

		for (int i = 0; i < count ; i++)
		{
			marble[i].Draw(hMemDC);
		}

		::Rectangle(hMemDC, info.left, info.top, info.right, info.bottom);
		for (int i = 0, j = 0; i < count; i++)
		{
			if (marble[i].select)
			{
				::Rectangle(hMemDC, 
					info.left + 20*(j%3), 
					info.top + 20*(j/3), 
					info.left + 20 + 20*(j%3), 
					info.top + 20 + 20*(j/3));

				RECT rcId = {info.left + 20*(j%3), 
					info.top + 20*(j/3), 
					info.left + 20 + 20*(j%3), 
					info.top + 20 + 20*(j/3)};

				//TCHAR szId[3];

				//szId[0] = _T('0') + marble[i].id/10;
				//szId[1] = _T('0') + marble[i].id%10;
				//szId[2] = _T('\0');

				//_stprintf_s(szId, 3, _T("%d"), marble[i].id);

				std::wostringstream oss;

				oss << marble[i].id;

				::DrawText(hMemDC, oss.str().c_str(), -1, &rcId, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				j++;
			}
		}


		if (drag)
		{
			POINT pt;
			::MoveToEx(hMemDC, box.left, box.top, &pt);
			::LineTo(hMemDC, box.left, box.bottom);
			::LineTo(hMemDC, box.right, box.bottom);
			::LineTo(hMemDC, box.right, box.top);
			::LineTo(hMemDC, box.left, box.top);
		}

		HDC hBitmapDC = ::CreateCompatibleDC(hMemDC);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hCursor);

		// draw
		::GdiTransparentBlt(hMemDC, ptMouse.x - 20, ptMouse.y - 20, 40, 41,
			hBitmapDC, rcCursor.left, rcCursor.top, rcCursor.right-rcCursor.left,
			rcCursor.bottom-rcCursor.top, RGB(0,0,255));

		::SelectObject(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);


		::BitBlt(hdc, 0, 0, rc.right-rc.left, rc.bottom-rc.top, hMemDC, 0, 0, SRCCOPY);

		::EndPaint(hWnd, &ps);

		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		if (drag)
		{
			::GetCursorPos(&ept);
			::ScreenToClient(hWnd, &ept);

			if (spt.x > ept.x)
			{
				box.left = ept.x;
				box.right = spt.x;
			}
			else
			{
				box.left = spt.x;
				box.right = ept.x;
			}

			if (spt.y > ept.y)
			{
				box.top = ept.y;
				box.bottom = spt.y;
			}
			else
			{
				box.top = spt.y;
				box.bottom = ept.y;
			}

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

			::GetCursorPos(&spt);
			::ScreenToClient(hWnd, &spt);
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