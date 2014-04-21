#pragma once

class InputProcessor
{
private :
	InputProcessor();
	~InputProcessor();

public :
	static InputProcessor& getReference()
	{
		static InputProcessor inst;

		return inst;
	}
	void Update(DWORD tick)
	{
		::GetKeyboardState(cKey);
	}
	bool IsPressed(const short& vkey)
	{
	}

private :
	enum {count=256};
	BYTE cKey[count];
};

#define InputDevice InputProcessor::getReference()