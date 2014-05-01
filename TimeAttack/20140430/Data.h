#pragma once

struct Data
{
	RECT rc;
	bool select;
	const LONG radius;
	POINT center;
	POINT destination;
	LONG update_dt;
	LONG update_delay;
	LONG speed; // 10pixel/s
	Data()
		: radius(20)
		, update_dt(0), update_delay(10), speed(100)
	{
		rc.left = rc.top = rc.right = rc.bottom = 0;
		select = false;
	}
	void SetPosition(const POINT& pos)
	{
		center = pos;
		destination = center;
		rc.left = pos.x - radius;
		rc.right = pos.x + radius;
		rc.top = pos.y - radius;
		rc.bottom = pos.y + radius;
	}
	void Draw(HDC hdc)
	{
		HBRUSH hBrush, hOldBrush;
		HPEN hPen, hOldPen;
		if (select)
		{
			hBrush = ::CreateSolidBrush(RGB(255,100,100));
			hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);
			hPen = ::CreatePen(PS_SOLID, 1, RGB(255,0,0));
			hOldPen = (HPEN)::SelectObject(hdc, hPen);
		}
		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
		if (select)
		{
			::SelectObject(hdc, hOldPen);
			::DeleteObject(hPen);

			::SelectObject(hdc, hOldBrush);
			::DeleteObject(hBrush);
		}
	}
	void SetDestination(const POINT& pt)
	{
		destination = pt;
	}
	bool Update(DWORD tick)
	{
		if (center.x == destination.x && center.y == destination.y)
		{
			update_dt = 0;
			return false;
		}

		if (update_dt > update_delay)
		{
			int count = update_dt/update_delay;

			for (int i = 0; i < count; i++)
			{
				POINT diff;
				diff.x = destination.x - center.x;
				diff.y = destination.y - center.y;

				if (diff.x > 0 && diff.x > update_delay*speed/1000)
				{
					center.x += update_delay*speed/1000;
				}
				else if (diff.x < 0 && diff.x < -update_delay*speed/1000)
				{
					center.x -= update_delay*speed/1000;
				}
				else
				{
					center.x = destination.x;
				}

				if (diff.y > 0 && diff.y > update_delay*speed/1000)
				{
					center.y += update_delay*speed/1000;
				}
				else if (diff.y < 0 && diff.y < -update_delay*speed/1000)
				{
					center.y -= update_delay*speed/1000;
				}
				else
				{
					center.y = destination.y;
				}

				rc.left = center.x - radius;
				rc.right = center.x + radius;
				rc.top = center.y - radius;
				rc.bottom = center.y + radius;

			}

			update_dt %= update_delay;
		}

		update_dt += tick;

		return true;
	}
};

