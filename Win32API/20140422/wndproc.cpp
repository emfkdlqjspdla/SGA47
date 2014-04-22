#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static HBITMAP hImage = NULL;
	static POINT ptMouse = {0,0};
	static SIZE cBitmap = {0,0};

	if (uMsg == WM_CREATE)
	{
		// 비트맵 이미지 로드.
		hImage = (HBITMAP)::LoadImage(NULL, 
					_T("1364722-starcraft2_marine.bmp"), 
					IMAGE_BITMAP,
					0,0,
					LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

		BITMAP bm;
		::GetObject(hImage, sizeof(BITMAP), &bm);
		cBitmap.cx = bm.bmWidth;
		cBitmap.cy = bm.bmHeight;

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		// 로드한 비트맵 이미지 핸들 제거.
		::DeleteObject(hImage);

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
			ptMouse.x - cBitmap.cx/2, 
			ptMouse.y - cBitmap.cy/2, 
			cBitmap.cx, cBitmap.cy, 
			hBitmapDC, 
			0, 0, 
			SRCCOPY);

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

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
