#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static POINT ptHour;
	static POINT ptMinute;
	static POINT ptSecond;

	static float theta_hour = 0.f;
	static float theta_minute = 0.f;
	static float theta_second = 0.f;
	static float hour_needle = 150;
	static float minute_needle = 120;
	static float second_needle = 100;

	if (uMsg == WM_CREATE)
	{
		::SetTimer(hWnd, 0, 1000, NULL);
	}
	else if (uMsg == WM_DESTROY)
	{
		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		SYSTEMTIME st;
		::GetLocalTime(&st);

		// 12 h : 360 = 1 h : x
		// x = 360/12 = 30

		theta_hour = (st.wHour%12)*30.f;

		ptHour.x = LONG(hour_needle*sin(theta_hour*D2R));
		ptHour.y = LONG(-hour_needle*cos(theta_hour*D2R));

		// 60 m : 360 = 1 m : x
		// x = 360/60 = 6

		theta_minute = (st.wMinute)*6.f;

		ptMinute.x = LONG(minute_needle*sin(theta_minute*D2R));
		ptMinute.y = LONG(-minute_needle*cos(theta_minute*D2R));
		
		theta_second = (st.wSecond)*6.f;

		ptSecond.x = LONG(second_needle*sin(theta_second*D2R));
		ptSecond.y = LONG(-second_needle*cos(theta_second*D2R));

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	else if (uMsg == WM_SIZE)
	{
		RECT rc = {0,0,400,400};

		DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		dwStyle = dwStyle & ~WS_MAXIMIZEBOX;
		dwStyle = dwStyle & ~WS_THICKFRAME;

		::AdjustWindowRect(&rc, dwStyle, FALSE);

		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;

		int cx = ::GetSystemMetrics(SM_CXSCREEN);
		int cy = ::GetSystemMetrics(SM_CYSCREEN);

		int x = (cx - width)/2;
		int y = (cy - height)/2;

		::SetWindowPos(hWnd, NULL, x, y,
			rc.right-rc.left, rc.bottom-rc.top, SWP_NOZORDER);

		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		// Handle Device Context
		HDC hdc = ::BeginPaint(hWnd, &ps);

		HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(0x24,0x55,0xAD));
		HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

		HBRUSH hBrush = ::CreateSolidBrush(RGB(0x64,0x95,0xED));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		LONG margin = 5;

		// dx, dy 만큼 RECT 를 키운다.
		// dx, dy 값이 음수이면 RECT를 줄이는 결과가 된다.
		::InflateRect(&rc, -margin, -margin);

		::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);


		LONG cx = (rc.right - rc.left)/2;
		LONG cy = (rc.bottom - rc.top)/2;

		POINT pt;
		::MoveToEx(hdc, cx, cy, &pt);
		::LineTo(hdc, cx + ptHour.x, cy + ptHour.y);

		::MoveToEx(hdc, cx, cy, &pt);
		::LineTo(hdc, cx + ptMinute.x, cy + ptMinute.y);

		::MoveToEx(hdc, cx, cy, &pt);
		::LineTo(hdc, cx + ptSecond.x, cy + ptSecond.y);

		::SelectObject(hdc, hOldBrush);
		::DeleteObject(hBrush);

		::SelectObject(hdc, hOldPen);
		::DeleteObject(hPen);

		::EndPaint(hWnd, &ps);
		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
