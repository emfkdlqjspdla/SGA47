#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static Image* Card[3] = {NULL};
	static int current = 0;

	if (uMsg == WM_CREATE)
	{
		for (int i = 0; i < 3; i++)
		{
			Card[i] = new Image;
			Card[i]->Load(_T("cards.bmp"), Rect(0 + 200*i, 0, 200 + 200*i, 400));
		}

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		for (int i = 0; i < 3; i++)
		{
			SafeDelete(Card[i]);
		}

		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_KEYDOWN)
	{
		if ((::GetAsyncKeyState(VK_RIGHT) & 0x8000) == 0x8000)
		{
			current++;
			if (current > 2)
				current = 2;

			Rect rc;
			::GetClientRect(hWnd, &rc);
			::InvalidateRect(hWnd, &rc, TRUE);
		}
		else if ((::GetAsyncKeyState(VK_LEFT) & 0x8000) == 0x8000)
		{
			current--;
			if (current < 0)
				current = 0;

			Rect rc;
			::GetClientRect(hWnd, &rc);
			::InvalidateRect(hWnd, &rc, TRUE);
		}

		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		Rect rcLeft(rc.center()<<Size(100,0), Size(200,400)*0.8f);
		Rect rcCenter(rc.center(), Size(200,400));
		Rect rcRight(rc.center()>>Size(100,0), Size(200,400)*0.8f);

		if (current-1 >= 0)
		{
			Card[current-1]->SetDrawRect(rcLeft);
			Card[current-1]->Draw(hdc);
		}
		if (current+1 <= 2)
		{
			Card[current+1]->SetDrawRect(rcRight);
			Card[current+1]->Draw(hdc);
		}

		Card[current]->SetDrawRect(rcCenter);
		Card[current]->Draw(hdc);


		::EndPaint(hWnd, &ps);

		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}