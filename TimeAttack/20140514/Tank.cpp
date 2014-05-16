#include "Tank.h"

Tank::Tank()
: radius(50), length(100), theta(0)
, update_dt(0), update_delay(100)
, input_dt(0), input_delay(100)
{
}
Tank::Tank(const Point& pt, const LONG& r)
: center(pt), radius(r), length(100), theta(0)
, update_dt(0), update_delay(100)
, input_dt(0), input_delay(100)
{
}
Tank::~Tank()
{
}
void Tank::Input(DWORD tick)
{
	InputDevice.Update(0);

	if (input_dt > input_delay)
	{
		if (InputDevice[VK_LEFT])
		{
			theta -= 10;
			if (theta == -360)
				theta = 0;
		}
		if (InputDevice[VK_RIGHT])
		{
			theta += 10;
			if (theta == 360)
				theta = 0;
		}
		if (InputDevice[VK_UP])
		{
			center.x = LONG(center.x + 5*cos((90-theta)*D2R));
			center.y = LONG(center.y - 5*sin((90-theta)*D2R));
			//center.y -= 5;
		}
		if (InputDevice[VK_DOWN])
		{
			center.x = LONG(center.x - 5*cos((90-theta)*D2R));
			center.y = LONG(center.y + 5*sin((90-theta)*D2R));
			//center.y += 5;
		}
		if (InputDevice[VK_SPACE])
		{
			Missile* pMissile = new Missile(last, 10, theta);

			MissileDepot.push_back(pMissile);
		}

		input_dt -= input_delay;
	}

	input_dt += tick;
}
void Tank::Update(DWORD tick)
{
	if (update_dt > update_delay)
	{

		last.x = LONG(center.x + length*cos((90-theta)*D2R));
		last.y = LONG(center.y - length*sin((90-theta)*D2R));

		MissileList::iterator it;
		for (it = MissileDepot.begin();
			it != MissileDepot.end();)
		{
			(*it)->Update(0);

			Point pt = (*it)->GetPosition();
			if (pt.x < 0 || pt.x > 400 ||
				pt.y < 0 || pt.y > 400)
			{
				SafeDelete(*it);

				it = MissileDepot.erase(it);
			}
			else
			{
				it++;
			}
		}


		update_dt -= update_delay;
	}

	update_dt += tick;

}
void Tank::Draw(HDC hdc)
{
	::Ellipse(hdc, 
		center.x - radius,
		center.y - radius,
		center.x + radius,
		center.y + radius);

	MissileList::iterator it;
	for (it = MissileDepot.begin();
		it != MissileDepot.end();
		it++)
	{
		if (*it != NULL)
			(*it)->Draw(hdc);
	}

	Point pt;
	::MoveToEx(hdc, center.x, center.y, &pt);
	::LineTo(hdc, last.x, last.y);
}
void Tank::SetPosition(const Point& pt)
{
	center = pt;
}