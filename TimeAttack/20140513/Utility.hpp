#pragma once

template<typename T>
void SafeDelete(T& pointer)
{
	if (pointer)
	{
		delete [] pointer;
		pointer = NULL;
	}
}

