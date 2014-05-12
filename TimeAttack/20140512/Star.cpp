#include "Star.h"

Star::Star(const LONG& _speed)
: hStar(NULL), speed(_speed)
, update_dt(0), delay(100)
{
	hStar = (HBITMAP)::LoadImage(NULL, _T("star.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	BITMAP bm;
	::GetObject(hStar, sizeof(bm), &bm);
	size = Size(bm.bmWidth, bm.bmHeight);

	LONG diff = rand()%20;
	drawSize = size*3/5 + Size(diff, diff);

	pos = Point(rand()%400, 0);
}
Star::~Star()
{
	if (hStar)
	{
		::DeleteObject(hStar);
		hStar = NULL;
	}

}
void Star::Update(DWORD tick)
{
	if (update_dt >= delay)
	{
		int count = update_dt / delay;

		for (int i = 0; i < count; i++)
		{
			pos.x -= 1*speed;
			pos.y += 2*speed;
		}

		update_dt %= delay;
	}

	update_dt += tick;
}
void Star::Draw(HDC hdc)
{
	HDC hBitmapDC = ::CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hStar);

	::GdiTransparentBlt(hdc, pos.x - drawSize.cx/2, pos.y - drawSize.cy/2,
		drawSize.cx, drawSize.cy,
		hBitmapDC, 0, 0, size.cx, size.cy, RGB(255,255,255));

	::SelectObject(hBitmapDC, hOldBitmap);
	::DeleteDC(hBitmapDC);
}
