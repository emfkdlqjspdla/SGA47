#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static HBITMAP hImage = NULL;
	static POINT ptMouse = {0,0};
	static SIZE cBitmap = {0,0};
	static int BitmapLeft = 0;

	if (uMsg == WM_CREATE)
	{
		// 비트맵 이미지 로드.
		hImage = (HBITMAP)::LoadImage(NULL, 
					_T("circle_group.bmp"), 
					IMAGE_BITMAP,
					0,0,
					LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

		BITMAP bm;
		::GetObject(hImage, sizeof(BITMAP), &bm);
		cBitmap.cx = bm.bmWidth;
		cBitmap.cy = bm.bmHeight;

		::SetTimer(hWnd, 0, 100, NULL);
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		// 로드한 비트맵 이미지 핸들 제거.
		::DeleteObject(hImage);

		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		// Handle Device Context
		HDC hdc = ::BeginPaint(hWnd, &ps);

		HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,200));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		::FillRect(hdc, &rc, hBrush);

		::SelectObject(hdc, hOldBrush);
		::DeleteObject(hBrush);

		// 이미지 DC 생성 및 이미지 선택
		HDC hBitmapDC = ::CreateCompatibleDC(hdc);
		HBITMAP hOldImage = (HBITMAP)::SelectObject(hBitmapDC, hImage);

		// 이미지 DC의 내용(이미지)을 화면에 뿌린다.
		::BitBlt(hdc, 
			ptMouse.x - 50, 
			ptMouse.y - 50, 
			100, 100, 
			hBitmapDC, 
			BitmapLeft, 0, 
			SRCCOPY);

		//::StretchBlt(hdc, 
		//	ptMouse.x - 50, 
		//	ptMouse.y - 50, 
		//	100, 100, 
		//	hBitmapDC, 
		//	0, 0,
		//	cBitmap.cx, cBitmap.cy,
		//	SRCCOPY);

		// 이미지 DC 제거.
		::SelectObject(hBitmapDC, hOldImage);
		::DeleteDC(hdc);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		ptMouse.x = GET_X_LPARAM(lParam);
		ptMouse.y = GET_Y_LPARAM(lParam);

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		InputDevice.Update(0);

		BitmapLeft += 100;
		if (BitmapLeft >= 700)
		{
			BitmapLeft = 0;
		}
		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		//if (InputDevice['1'])
		//{
		//	BitmapLeft = 0;
		//	RECT rc;
		//	::GetClientRect(hWnd, &rc);
		//	::InvalidateRect(hWnd, &rc, TRUE);
		//}
		//if (InputDevice['2'])
		//{
		//	BitmapLeft = 100;
		//	RECT rc;
		//	::GetClientRect(hWnd, &rc);
		//	::InvalidateRect(hWnd, &rc, TRUE);
		//}
		//if (InputDevice['3'])
		//{
		//	BitmapLeft = 200;
		//	RECT rc;
		//	::GetClientRect(hWnd, &rc);
		//	::InvalidateRect(hWnd, &rc, TRUE);
		//}
		//if (InputDevice['4'])
		//{
		//	BitmapLeft = 300;
		//	RECT rc;
		//	::GetClientRect(hWnd, &rc);
		//	::InvalidateRect(hWnd, &rc, TRUE);
		//}
		//if (InputDevice['5'])
		//{
		//	BitmapLeft = 400;
		//	RECT rc;
		//	::GetClientRect(hWnd, &rc);
		//	::InvalidateRect(hWnd, &rc, TRUE);
		//}
		//if (InputDevice['6'])
		//{
		//	BitmapLeft = 500;
		//	RECT rc;
		//	::GetClientRect(hWnd, &rc);
		//	::InvalidateRect(hWnd, &rc, TRUE);
		//}
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
