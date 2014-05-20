#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_RBUTTONDOWN)
	{
		FontSelector();

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

		::SetBkMode(hdc, TRANSPARENT);

		HFONT hFont = ::CreateFontIndirect(&LOGFONT(FontSelector));
		HFONT hOldFont = (HFONT)::SelectObject(hdc, hFont);

		::DrawText(hdc, _T("Lim Jong Kyu"), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		::SelectObject(hdc, hOldFont);
		::DeleteObject(hFont);

		::EndPaint(hWnd, &ps);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}