#include "Text.h"

Text::Text()
: text(NULL), color(RGB(0,0,0))
{
}
Text::Text(const Text& o)
: text(NULL)
{
	*this = o;
}
Text::~Text()
{
	::SafeDelete(text);
}

void Text::Draw(HDC hdc)
{
	::DrawText(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void Text::Set(const TCHAR* str, const COLORREF& clr, const Rect& r)
{
	::strAlloc(text, str);
	color = clr;
	rc = r;
}
const TCHAR* Text::GetString() const
{
	return text;
}
const COLORREF Text::GetColor() const
{
	return color;
}
const Rect Text::GetRect() const
{
	return rc;
}

Text& Text::operator = (const Text& o)
{
	::strAlloc(text, o.text);
	color = o.color;
	rc = o.rc;

	return *this;
}
Text& Text::operator = (const TCHAR* str)
{
	::strAlloc(text, str);

	return *this;
}
Text& Text::operator += (const TCHAR* str)
{
	::strAppend(text, str);

	return *this;
}
Text& Text::operator << (const TCHAR* str)
{
	::strAppend(text, str);

	return *this;
}
Text& Text::operator << (const COLORREF& clr)
{
	color = clr;

	return *this;
}
Text& Text::operator << (const Rect& r)
{
	rc = r;

	return *this;
}

TextManager::TextManager()
{}
TextManager::~TextManager()
{}

void TextManager::Attach(HWND hWnd)
{
	hOwner = hWnd;
}
void TextManager::Input(DWORD tick)
{
}
void TextManager::Update(DWORD tick)
{
}
void TextManager::Draw(HDC hdc)
{
	Rect rc;
	::GetClientRect(hOwner, &rc);

	List::iterator it;
	for (it = textDepot.begin(); it != textDepot.end(); it++)
	{
		Rect tmp;
		if (::IntersectRect(&tmp, &rc, &((*it)->GetRect())))
		{
			(*it)->Draw(hdc);
		}
	}
}
void TextManager::AddText(Text* pText)
{
	textDepot.push_back(pText);
}
void TextManager::RemoveText(Text* pText)
{
	List::iterator it;
	for (it = textDepot.begin(); it != textDepot.end(); it++)
	{
		if ((*it) == pText)
		{
			delete (*it);

			it = textDepot.erase(it);
			break;
		}
	}
}