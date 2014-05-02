#pragma once

struct Circle
{
	POINT center;
	LONG radius;
	bool select;
	int id;

	Circle():radius(20){}
	void Draw(HDC hdc)
	{
		HBRUSH hBrush = NULL;
		HBRUSH hOldBrush = NULL;
		HPEN hPen = NULL;
		HPEN hOldPen = NULL;
		if (select)
		{
			hBrush = ::CreateSolidBrush(RGB(255,0,0));
			hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

			hPen = ::CreatePen(PS_SOLID, 1, RGB(255,200,200));
			hOldPen = (HPEN)::SelectObject(hdc, hPen);
		}

		::Ellipse(hdc, center.x - radius, center.y - radius,
			center.x + radius, center.y + radius);

		if (select)
		{
			::SelectObject(hdc, hOldPen);
			::DeleteObject(hPen);

			::SelectObject(hdc, hOldBrush);
			::DeleteObject(hBrush);
		}
	}
};
