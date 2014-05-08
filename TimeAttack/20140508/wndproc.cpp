#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static HBITMAP hSample[CURSOR_MAX] = {NULL};
	static RECT rcSample[CURSOR_MAX];
	static int MouseState = CURSOR_NORMAL;
	static POINT ptMouse = {0,0};
	static bool bClipMouse = true;

	if (uMsg == WM_CREATE)
	{
		hSample[CURSOR_NORMAL] = (HBITMAP)::LoadImage(NULL, _T("sample.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
		hSample[CURSOR_VERT] = (HBITMAP)::LoadImage(NULL, _T("sample.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
		hSample[CURSOR_HORT] = (HBITMAP)::LoadImage(NULL, _T("sample.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

		RECT rcTemp = {0,0,100,100};
		rcSample[CURSOR_NORMAL] = rcTemp;
		::OffsetRect(&rcTemp, 100, 0);
		rcSample[CURSOR_VERT] = rcTemp;
		::OffsetRect(&rcTemp, 100, 0);
		rcSample[CURSOR_HORT] = rcTemp;

		RECT rc;
		::GetClientRect(hWnd, &rc);
		LONG cx = rc.right - rc.left;
		LONG cy = rc.bottom - rc.top;

		ptMouse.x = cx/2;
		ptMouse.y = cy/2;

		POINT ptScreen = ptMouse;
		::ClientToScreen(hWnd, &ptScreen);

		::SetCursorPos(ptScreen.x, ptScreen.y);
		::ShowCursor(FALSE);

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::ShowCursor(TRUE);

		for (int i = 0; i < CURSOR_MAX; i++)
		{
			::DeleteObject(hSample[i]);
		}

		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		HDC hBitmapDC = ::CreateCompatibleDC(hdc);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hSample[MouseState]);

		::GdiTransparentBlt(hdc, 
			ptMouse.x - 50, ptMouse.y - 50, 
			100, 100,
			hBitmapDC,
			rcSample[MouseState].left, rcSample[MouseState].top,
			rcSample[MouseState].right - rcSample[MouseState].left,
			rcSample[MouseState].bottom - rcSample[MouseState].top,
			RGB(255,255,255));

		::SelectObject(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		const int margin = 10;
		RECT rc;
		::GetClientRect(hWnd, &rc);

		if (rc.top + margin > ptMouse.y || rc.bottom - margin < ptMouse.y)
			MouseState = CURSOR_VERT;
		else if (rc.left + margin > ptMouse.x || rc.right - margin < ptMouse.x)
			MouseState = CURSOR_HORT;
		else
			MouseState = CURSOR_NORMAL;

		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	else if (uMsg == WM_KEYDOWN)
	{
		if ((::GetAsyncKeyState(VK_ESCAPE) & 0x8000) == 0x8000)
		{
			bClipMouse = !bClipMouse;
			if (bClipMouse)
			{
				RECT rc;
				::GetClientRect(hWnd, &rc);

				POINT lt = {rc.left, rc.top};
				POINT rb = {rc.right, rc.bottom};

				::ClientToScreen(hWnd, &lt);
				::ClientToScreen(hWnd, &rb);

				RECT rcScreen = {lt.x, lt.y, rb.x, rb.y};


				::ClipCursor(&rcScreen);
				::ShowCursor(FALSE);
			}
			else
			{
				::ClipCursor(NULL);
				::ShowCursor(TRUE);
			}
		}

	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}