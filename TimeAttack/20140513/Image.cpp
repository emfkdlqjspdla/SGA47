#include "Image.h"

Image::Image()
: hBitmap(NULL)
{
}
Image::~Image()
{
	if (hBitmap)
		::DeleteObject(hBitmap);
}

bool Image::Load(LPCTSTR lpFileName)
{
	hBitmap = (HBITMAP)::LoadImage(NULL, 
								lpFileName, 
								IMAGE_BITMAP,
								0, 0, 
								LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
	if (hBitmap == NULL)
		return false;

	BITMAP bm;
	::GetObject(hBitmap, sizeof(bm), &bm);

	rcSrc = Rect(Point(0,0), Point(bm.bmWidth, bm.bmHeight));

	return true;
}
bool Image::Load(LPCTSTR lpFileName, const Rect& rc)
{
	hBitmap = (HBITMAP)::LoadImage(NULL, lpFileName, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
	if (hBitmap == NULL)
		return false;

	rcSrc = rc;

	return true;
}
void Image::Draw(HDC hdc)
{
	if (!hBitmap) return;

	HDC hBitmapDC = ::CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hBitmap);

	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.BlendOp = 0;
	bf.SourceConstantAlpha = 255;

	::GdiAlphaBlend(hdc,
		rcDraw.left, rcDraw.top,
		rcDraw.width(), rcDraw.height(),
		hBitmapDC,
		rcSrc.left, rcSrc.top,
		rcSrc.width(), rcDraw.height(),
		bf);

	::SelectObject(hBitmapDC, hOldBitmap);
	::DeleteDC(hBitmapDC);
}
void Image::SetDrawRect(const Rect& rc)
{
	rcDraw = rc;
}
