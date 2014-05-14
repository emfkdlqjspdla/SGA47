#include "Star.h"

Star::Star(const Point& pt, const LONG& _speed, const LONG& _th)
: hStar(NULL), speed(_speed)
, update_dt(0), delay(100)
, pos(pt), theta(_th)
, alpha(255)
{
	hStar = (HBITMAP)::LoadImage(NULL, _T("star.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	BITMAP bm;
	::GetObject(hStar, sizeof(bm), &bm);
	size = Size(bm.bmWidth, bm.bmHeight);

	LONG diff = rand()%20;
	drawSize = size*(3.f/5.f) + Size(diff, diff);
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
			pos.x = pos.x + speed*cos((90-theta)*D2R);
			pos.y = pos.y - speed*sin((90-theta)*D2R);

			alpha = alpha*9/10;

			drawSize = drawSize*0.95f;
		}

		update_dt %= delay;
	}

	update_dt += tick;
}
void Star::Draw(HDC hdc)
{
	HDC hBitmapDC = ::CreateCompatibleDC(hdc);
	HBITMAP hBackBitmap = ::CreateCompatibleBitmap(hdc, size.cx, size.cy);
	HBITMAP hOldBackBitmap = (HBITMAP)::SelectObject(hBitmapDC, hBackBitmap);

	HDC hStarDC = ::CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hStarDC, hStar);

	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.BlendOp = 0;
	bf.SourceConstantAlpha = alpha;

	::GdiAlphaBlend(hBitmapDC,
					0, 0,
					size.cx, size.cy,
					hStarDC,
					0, 0, 
					size.cx, size.cy, 
					bf);

	::SelectObject(hStarDC, hOldBitmap);
	::DeleteDC(hStarDC);

	COLORREF dest = ::GetPixel(hBitmapDC, 0,0);

	::GdiTransparentBlt(hdc, 
		pos.x - drawSize.cx/2, pos.y - drawSize.cx/2,
		drawSize.cx, drawSize.cy, 
		hBitmapDC, 
		0, 0, 
		size.cx, size.cy, 
		dest);

	::SelectObject(hBitmapDC, hOldBackBitmap);
	::DeleteObject(hBackBitmap);
	::DeleteDC(hBitmapDC);
}
Point Star::GetPosition() const
{
	return pos;
}
