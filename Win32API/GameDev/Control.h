#pragma once

#include <windows.h>

class Control
{
	typedef unsigned long id_type;
public :
	Control();
	virtual ~Control();

	void Attach(HWND);

	virtual void Input(DWORD tick);
	virtual void Update(DWORD tick);
	virtual void Draw(HDC hdc);

	id_type id() const;
private :
	id_type cid;

	static id_type gen_id;

protected :
	HWND hOwner;
};
