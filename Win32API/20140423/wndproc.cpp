#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static Object* marble[10];

	if (uMsg == WM_CREATE)
	{
		// 원을 만들기
		for (int i = 0; i < 10; i++)
		{
			marble[i] = new Circle;
		}

		::SetTimer(hWnd, 0, 30, NULL);
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		// 만든 원을 삭제..
		for (int i = 0; i < 10; i++)
		{
			delete marble[i];
		}

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

		// 만든 원 그리기..
		for (int i = 0; i < 10; i++)
		{
			marble[i]->Draw(hdc);
		}

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		// 만든 원 업데이트.
		for (int i = 0; i < 10; i++)
		{
			marble[i]->Update(0);
		}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
