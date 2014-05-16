#pragma once

#include <windows.h>

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

template<typename GDIObject>
GDIObject Select(HDC hdc, GDIObject obj)
{
	return (GDIObject)::SelectObject(hdc, obj);
}

template<typename T>
void SafeDelete(T& pointer)
{
	if (pointer)
	{
		delete [] pointer;
		pointer = NULL;
	}
}

template<typename CharType>
CharType* strAlloc(CharType* & dest, const CharType* & src)
{
	SafeDelete(dest);

	unsigned int i = 0;
	while (src[i++]);

	dest = new CharType[i];

	while (dest[i++] = src[i]);

	return dest;
}