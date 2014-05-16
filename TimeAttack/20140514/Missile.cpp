#include "Missile.h"

Missile::Missile()
: radius(10), theta(0)
{
}
Missile::Missile(const Point& pt, const LONG& r, const LONG& th)
: center(pt), radius(r), theta(th)
{
}

void Missile::Update(DWORD tick)
{
	center.x = LONG(center.x + 20*cos((90-theta)*D2R));
	center.y = LONG(center.y - 20*sin((90-theta)*D2R));
}
void Missile::Draw(HDC hdc)
{
	::Ellipse(hdc, 
		center.x - radius,
		center.y - radius,
		center.x + radius,
		center.y + radius);
}
void Missile::SetPosition(const Point& pt)
{
	center = pt;
}
void Missile::SetTheta(const LONG& th)
{
	theta = th;
}
Point Missile::GetPosition() const
{
	return center;
}
