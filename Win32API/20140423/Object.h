#pragma once

class Object
{
public :
	virtual void Update(DWORD tick) = 0;
	virtual void Draw(HDC hdc) = 0;
};


class Circle : public Object
{
public :
	Circle(HWND hWnd = NULL)
	{
		hOwner = hWnd;
		velocity = 0;

		ptCenter.x = rand()%300 + 50;
		ptCenter.y = rand()%100 + 50;

		radius = rand()%50 + 10;

		BYTE r = rand()%256;
		BYTE g = rand()%256;
		BYTE b = rand()%256;

		color = RGB(r,g,b);
	}
	void SetCenter()
	{
	}
	void Attach(HWND hWnd)
	{
		hOwner = hWnd;
	}
	void Update(DWORD tick)
	{
		::GetCursorPos(&ptCenter);
		::ScreenToClient(hOwner, &ptCenter);


		//velocity += 1;
		//ptCenter.y += velocity;

		//if (ptCenter.y + radius >= 400)
		//{
		//	velocity = -velocity - 1;
		//}
	}
	void Draw(HDC hdc)
	{
		HBRUSH hBrush = ::CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		::Ellipse(hdc, 
			ptCenter.x - radius,
			ptCenter.y - radius,
			ptCenter.x + radius,
			ptCenter.y + radius);

		::SelectObject(hdc, hOldBrush);
		::DeleteObject(hBrush);
	}

private :
	POINT ptCenter;
	LONG radius;
	LONG velocity;
	COLORREF color;
	HWND hOwner;
};