#pragma once

#include <windows.h>
#include <tchar.h>
#include <list>
#include "BaseType.h"
#include "Utility.hpp"

class Text
{
public :
	Text();
	Text(const Text&);
	virtual ~Text();

	void Draw(HDC);

	void Set(const TCHAR*, const COLORREF&, const Rect&);
	const TCHAR* GetString() const;
	const COLORREF GetColor() const;
	const Rect GetRect() const;

	Text& operator = (const Text&);
	Text& operator = (const TCHAR*);
	Text& operator += (const TCHAR*);
	Text& operator << (const TCHAR*);
	Text& operator << (const COLORREF&);
	Text& operator << (const Rect&);

private :
	TCHAR* text;
	COLORREF color;
	Rect rc;
};

class TextManager : public singleton<TextManager>
{
	friend class singleton<TextManager>;

	typedef std::list<Text*> List;
private :
	TextManager();
	~TextManager();

public :	
	void Attach(HWND);

	void Input(DWORD);
	void Update(DWORD);
	void Draw(HDC);

	void AddText(Text*);
	void RemoveText(Text*);

private :
	HWND hOwner;
	List textDepot;
};