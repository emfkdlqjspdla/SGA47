#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static std::list<Image*> CardList;
	static int current = 0;
	static int addindex = 0;
	static HDC hMainDC = NULL;
	static HDC hMemDC = NULL;
	static HBITMAP hMemBitmap = NULL;
	static HBITMAP hOldBitmap = NULL;

	if (uMsg == WM_CREATE)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		hMainDC = ::GetDC(hWnd);
		hMemDC = ::CreateCompatibleDC(hMainDC);
		hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rc.width(), rc.height());
		hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

		Image* pCard = new Image;
		pCard->Load(_T("cards.bmp"), Rect(0 + 200*(addindex%3), 0, 200 + 200*(addindex%3), 400));
		addindex++;

		CardList.push_back(pCard);

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		std::list<Image*>::iterator it;
		for (it = CardList.begin(); it != CardList.end();)
		{
			delete *it;
			it = CardList.erase(it);
		}

		::SelectObject(hMemDC, hOldBitmap);
		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);
		::ReleaseDC(hWnd, hMainDC);

		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_KEYDOWN)
	{
		if ((::GetAsyncKeyState(VK_RIGHT) & 0x8000) == 0x8000)
		{
			current++;
			if (current > int(CardList.size())-1)
				current = CardList.size()-1;

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
		else if ((::GetAsyncKeyState(VK_ADD) & 0x8000) == 0x8000)
		{
			Image* pCard = new Image;
			pCard->Load(_T("cards.bmp"), Rect(0 + 200*(addindex%3), 0, 200 + 200*(addindex%3), 400));
			addindex++;

			CardList.push_back(pCard);

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

		::SetDCBrushColor(hMemDC, RGB(255,255,255));
		::FillRect(hMemDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

		Rect rcCenter(rc.center(), Size(150,300));

		DrawCard(hMemDC, CardList, rcCenter, current, current);

		::BitBlt(hdc, 0, 0, rc.width(), rc.height(), hMemDC, 0, 0, SRCCOPY);

		::EndPaint(hWnd, &ps);

		return 0;
	}
	else if (uMsg == WM_ERASEBKGND)
	{
		return 1;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void DrawCard(HDC hdc, std::list<Image*>& depot, const Rect& rc, const int& current, const int& center, const BYTE& alpha/*=255*/)
{
	Rect rcLeft(rc.center()<<Size(rc.width()/2, 0), Size(rc.width(), rc.height())*0.8f);
	Rect rcRight(rc.center()>>Size(rc.width()/2, 0), Size(rc.width(), rc.height())*0.8f);

	if (current-1 < center && current - 1 >= 0)
	{
		if (alpha*2/3 > 0)
			DrawCard(hdc, depot, rcLeft, current-1, center, alpha*2/3);
	}

	if (current+1 > center && current + 1 < int(depot.size()))
	{
		if (alpha*2/3 > 0)
			DrawCard(hdc, depot, rcRight, current+1, center, alpha*2/3);
	}

	std::list<Image*>::iterator it = depot.begin();
	std::advance(it, current);
	if (it != depot.end())
	{
		Image* pImage = *it;

		pImage->SetDrawRect(rc);
		pImage->SetAlphaValue(alpha);
		pImage->Draw(hdc);
	}
}
