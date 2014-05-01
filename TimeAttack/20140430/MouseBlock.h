#pragma once

struct MouseBlock
{
	MouseBlock()
	{
	}
	~MouseBlock()
	{
	}
	void Attach(HWND hWnd)
	{
		if (hWnd == NULL) return;

		hOwner = hWnd;

		hMainDC = ::GetDC(hWnd);
		::GetClientRect(hWnd, &rc);

		cx = rc.right - rc.left;
		cy = rc.bottom - rc.top;

		hTransSrcDC = ::CreateCompatibleDC(hMainDC);
		hTransBitmap = ::CreateCompatibleBitmap(hMainDC, cx, cy);
		hOldTransBitmap = (HBITMAP)::SelectObject(hTransSrcDC, hTransBitmap);

		hAlphaSrcDC = ::CreateCompatibleDC(hMainDC);
		hAlphaBitmap = ::CreateCompatibleBitmap(hMainDC, cx, cy);
		hOldAlphaBitmap = (HBITMAP)::SelectObject(hAlphaSrcDC, hAlphaBitmap);
	}
	void Detach()
	{
		::SelectObject(hAlphaSrcDC, hOldAlphaBitmap);
		::DeleteObject(hAlphaBitmap);
		::DeleteDC(hAlphaSrcDC);

		::SelectObject(hTransSrcDC, hOldTransBitmap);
		::DeleteObject(hTransBitmap);
		::DeleteDC(hTransSrcDC);

		::ReleaseDC(hOwner, hMainDC);
	}

	void SetEndPoint(const POINT& pt)
	{
		ept = pt;

		if (spt.x > ept.x)
		{
			box.left = ept.x;
			box.right = spt.x;
		}
		else
		{
			box.left = spt.x;
			box.right = ept.x;
		}

		if (spt.y > ept.y)
		{
			box.top = ept.y;
			box.bottom = spt.y;
		}
		else
		{
			box.top = spt.y;
			box.bottom = ept.y;
		}
	}
	void SetStartPoint(const POINT& pt)
	{
		spt = pt;
	}
	inline RECT GetBox() const
	{
		return box;
	}
	void Draw(HDC hdc)
	{
		::SetDCBrushColor(hTransSrcDC, RGB(255,255,255));
		::FillRect(hTransSrcDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

		HBRUSH hBrush = ::CreateSolidBrush(RGB(100,149,237));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hTransSrcDC, hBrush);

		HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0,255,255));
		HPEN hOldPen = (HPEN)::SelectObject(hTransSrcDC, hPen);

		::Rectangle(hTransSrcDC, box.left, box.top, box.right, box.bottom);

		::SelectObject(hTransSrcDC, hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hTransSrcDC, hOldBrush);
		::DeleteObject(hBrush);

		::GdiTransparentBlt(hAlphaSrcDC, 0, 0, cx, cy,
			hTransSrcDC, 0, 0, cx, cy, RGB(255,255,255));

		BLENDFUNCTION bf;
		bf.AlphaFormat = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.BlendOp = 0;
		bf.SourceConstantAlpha = 127;

		::GdiAlphaBlend(hdc, 
			box.left, box.top, 
			box.right - box.left, 
			box.bottom - box.top,
			hAlphaSrcDC, 
			box.left, box.top, 
			box.right - box.left, 
			box.bottom - box.top, bf);
	}
private :
	HWND hOwner;
	HDC hMainDC;
	RECT rc;
	int cx;
	int cy;

	////////////////////////////////////
	// transparent
	HDC hTransSrcDC;
	HBITMAP hTransBitmap;
	HBITMAP hOldTransBitmap;

	////////////////////////////////////
	// alpha
	HDC hAlphaSrcDC;
	HBITMAP hAlphaBitmap;
	HBITMAP hOldAlphaBitmap;

	////////////////////////////////////
	// rectangle
	POINT ept;
	POINT spt;
	RECT box;
};
