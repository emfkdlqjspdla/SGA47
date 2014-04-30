#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static HBITMAP hGray;
	static bool bControlMode = true;
	static BYTE alpha = 0;
	static bool dir = true;

	if (uMsg == WM_CREATE)
	{
		hGray = (HBITMAP)::LoadImage(NULL, _T("gray.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

		::SetTimer(hWnd, 0, 10, NULL);
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::KillTimer(hWnd, 0);
		::DeleteObject(hGray);

		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;

		PAINTSTRUCT ps;
		// Handle Device Context
		HDC hdc = ::BeginPaint(hWnd, &ps);

		::DrawText(hdc, _T("Hello World"), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		HDC hBitmapDC = ::CreateCompatibleDC(hdc);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hGray);

		BITMAP bm;
		::GetObject(hGray, sizeof(BITMAP), &bm);
		int bx = bm.bmWidth;
		int by = bm.bmHeight;

		if (bControlMode)
		{
			BLENDFUNCTION bf;
			bf.BlendFlags = 0;
			bf.BlendOp = AC_SRC_OVER;
			bf.AlphaFormat = 0;
			bf.SourceConstantAlpha = alpha;

			::GdiAlphaBlend(hdc, 0, 0, cx, cy, hBitmapDC, 0, 0, bx, by, bf);

			//::StretchBlt(hdc, 0, 0, cx, cy, hBitmapDC, 0, 0, bx, by, SRCCOPY);
		}

		::SelectObject(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	//else if (uMsg == WM_ERASEBKGND)
	//{
	//	return 1;
	//}
	else if (uMsg == WM_KEYDOWN)
	{
		bControlMode = !bControlMode;

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		if (dir == true && alpha == 255)
			dir = false;
		else if (dir == false && alpha == 0)
			dir = true;

		if (dir)
			alpha += 1;
		else
			alpha -= 1;

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
