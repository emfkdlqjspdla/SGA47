#pragma once

#include <sstream>
#include "BaseType.h"
#include "InputDevice.h"
#include "utility.hpp"

class Box
{
public :
	Box();
	virtual ~Box();
	void Attach(HWND hWnd);

	void Input(DWORD);
	void Update(DWORD);
	void Draw(HDC);

	void SetDrawRect(const Rect& rc);
	void SetColor(const COLORREF clr);

	Rect GetDrawRect() const;
	COLORREF GetColor() const;

	bool IsSelect() const;

private :
	HWND hOwner;
	Rect rcDraw;
	COLORREF color;
	bool bSelect;

	int id;

	static int gen_id;
};