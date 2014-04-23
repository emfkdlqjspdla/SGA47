#pragma once

#include "utility.hpp"
#include <windows.h>

class InputProcessor : public singleton<InputProcessor>
{
	friend class singleton<InputProcessor>;
private :
	InputProcessor();
	~InputProcessor();

public :
	void Update(DWORD tick);
	bool IsPressed(const short& vkey);
	bool operator [] (const short& vkey);

private :
	enum {count=256};
	BYTE cKey[count];
};

#define InputDevice InputProcessor::getReference()