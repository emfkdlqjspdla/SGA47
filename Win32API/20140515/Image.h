#pragma once

#include <windows.h>
#include <tchar.h>
#include "../GameDev/GameDev.h"

class Image
{
public :
	Image();
	virtual ~Image();

public :
	bool Load(LPCTSTR lpFileName);
	bool Load(LPCTSTR lpFileName, const Rect& rc);
	void Draw(HDC hdc);

	void SetDrawRect(const Rect& rc);
	void SetAlphaValue(const BYTE& _alpha);
	void SetTransparentColor(const COLORREF& clr);

private :
	HBITMAP hBitmap;
	Rect rcDraw;
	Rect rcSrc;

	BYTE alpha;
	bool bUseAlpha;

	COLORREF clrTransparent;
	bool bUseTransparent;
};