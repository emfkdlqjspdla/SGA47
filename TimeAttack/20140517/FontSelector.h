#pragma once

#ifdef _DEBUG
#pragma comment(lib, "Library/freetype250_D.lib")
#else
#pragma comment(lib, "Library/freetype250.lib")
#endif //_DEBUG

#include "ft2build.h"
#include FT_FREETYPE_H
#include <windows.h>
#include <tchar.h>
#include <fstream>
#include "utility.hpp"

class FontProcessor : public singleton<FontProcessor>
{
	friend class singleton<FontProcessor>;
	enum {Win32, FreeType};
private :
	FontProcessor(const int& _mode = FreeType);
	~FontProcessor();

public  :
	void Attach(HWND);

	operator LOGFONT ();
	void operator ()();

private :
	void _loadWin32();
	void _saveWin32();
	void _loadFreetype();
	void _saveFreetype();
private :
	HWND hOwner;
	LOGFONT logFont;
	bool bLoaded;
	const char* strSaveFileName;

	FT_Library library;
	bool bInitFreetype;
	FT_Face fontface;
	int xres;
	int yres;

	int mode;
};

#define FontSelector FontProcessor::getReference()