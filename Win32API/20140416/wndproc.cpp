#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		// Handle Device Context
		HDC hdc = ::BeginPaint(hWnd, &ps);

		// GDI object
		// pen, brush, font, path, region, pallete
		RECT rc;
		::GetClientRect(hWnd, &rc);

		HBRUSH hRedBrush = ::CreateSolidBrush(RGB(0xFF,0x00,0x00));
		HBRUSH hBlueBrush = ::CreateSolidBrush(RGB(0x00,0x00,0xFF));
		HBRUSH hGreenBrush = ::CreateSolidBrush(RGB(0x00,0xFF,0x00));

		BYTE r = rand()%256;
		BYTE g = rand()%256;
		BYTE b = rand()%256;
		HBRUSH hRandBrush = ::CreateSolidBrush(RGB(r,g,b));

		RECT rc1 = rc;
		rc1.right = rc1.right/2;
		rc1.bottom = rc1.bottom/2;


		RECT rc2 = rc1;
		::OffsetRect(&rc2, (rc.right-rc.left)/2, 0);

		RECT rc3 = rc1;
		::OffsetRect(&rc3, 0, (rc.bottom-rc.top)/2);

		RECT rc4 = rc1;
		::OffsetRect(&rc4, (rc.right-rc.left)/2, (rc.bottom-rc.top)/2);

		::FillRect(hdc, &rc1, hRedBrush);
		::FillRect(hdc, &rc2, hBlueBrush);
		::FillRect(hdc, &rc3, hGreenBrush);
		::FillRect(hdc, &rc4, hRandBrush);

		//::SetBkMode(hdc, TRANSPARENT);
		//::DrawText(hdc, _T("Hello Win32 Programming"), -1, 
		//	&rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		::DeleteObject(hRedBrush);
		::DeleteObject(hBlueBrush);
		::DeleteObject(hGreenBrush);
		::DeleteObject(hRandBrush);

		::EndPaint(hWnd, &ps);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
