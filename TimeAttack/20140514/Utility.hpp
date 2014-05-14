#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

template<typename T>
class singleton
{
protected :
	singleton(){}
	virtual ~singleton(){}

public :
	static T& getReference()
	{
		static T inst;
		return inst;
	}
};

template<typename T>
void SafeDelete(T& pointer)
{
	if (pointer)
	{
		delete pointer;
		pointer = NULL;
	}
}


// pi(radian) : 180(degree) = x(radian) : 1(degree)
// 180 * x = pi
// x = pi/180

#define D2R (M_PI/180.)