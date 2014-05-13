#pragma once

#include <windows.h>
#include <tchar.h>
#include "BaseType.h"

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

private :
	HBITMAP hBitmap;
	Rect rcDraw;
	Rect rcSrc;
};