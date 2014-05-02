#pragma once

struct Circle
{
	POINT center;
	LONG radius;
	bool select;
	int id;
	HBITMAP hSCV;
	RECT rcSCV;

	Circle():radius(20)
	{
		hSCV = (HBITMAP)::LoadImage(NULL, _T("scv.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

		::SetRect(&rcSCV, 306, 9, 340, 49);
	}
	~Circle()
	{
		::DeleteObject(hSCV);
	}
	void Draw(HDC hdc)
	{
		if (select)
		{
			::Ellipse(hdc, center.x - radius, center.y - radius,
				center.x + radius, center.y + radius);
		}
		HDC hBitmapDC = ::CreateCompatibleDC(hdc);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hSCV);

		// draw
		::GdiTransparentBlt(hdc, center.x - 17, center.y - 20, 35, 41,
			hBitmapDC, rcSCV.left, rcSCV.top, rcSCV.right-rcSCV.left,
			rcSCV.bottom-rcSCV.top, RGB(48,64,47));

		::SelectObject(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);

		

		//HBRUSH hBrush = NULL;
		//HBRUSH hOldBrush = NULL;
		//HPEN hPen = NULL;
		//HPEN hOldPen = NULL;
		//if (select)
		//{
		//	hBrush = ::CreateSolidBrush(RGB(255,0,0));
		//	hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		//	hPen = ::CreatePen(PS_SOLID, 1, RGB(255,200,200));
		//	hOldPen = (HPEN)::SelectObject(hdc, hPen);
		//}

		//::Ellipse(hdc, center.x - radius, center.y - radius,
		//	center.x + radius, center.y + radius);

		//if (select)
		//{
		//	::SelectObject(hdc, hOldPen);
		//	::DeleteObject(hPen);

		//	::SelectObject(hdc, hOldBrush);
		//	::DeleteObject(hBrush);
		//}
	}
};
