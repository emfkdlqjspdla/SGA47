#include "Object.h"

bool IsCollide(Object* lhs, Object* rhs)
{
	POINT ptCenterL = lhs->getCenter();
	POINT ptCenterR = rhs->getCenter();

	LONG rL = rhs->getRadius();
	LONG lL = lhs->getRadius();

	LONG dx = ptCenterL.x - ptCenterR.x;
	LONG dy = ptCenterL.y - ptCenterR.y;

	float D = sqrt(float(dx*dx + dy*dy));

	if (D <= rL + lL)
		return true;

	return false;
}