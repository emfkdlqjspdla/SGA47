#include "FontSelector.h"

FontProcessor::FontProcessor(const int& _mode/*=FreeType*/)
: hOwner(NULL), strSaveFileName("font.cfg")
, bLoaded(false), bInitFreetype(false)
, mode(_mode)
{
	FT_Error err = ::FT_Init_FreeType(&library);
	if (err)
	{
		bInitFreetype = false;
	}

	if (mode == Win32)
		_loadWin32();
	else if (mode == FreeType)
		_loadFreetype();
}
FontProcessor::~FontProcessor()
{
	if (mode == Win32)
		_saveWin32();
	else if (mode == FreeType)
		_saveFreetype();
}

void FontProcessor::Attach(HWND hWnd)
{
	hOwner = hWnd;

	HDC hdc = ::GetDC(hOwner);
	xres = ::GetDeviceCaps(hdc, HORZRES);
	yres = ::GetDeviceCaps(hdc, VERTRES);

	::ReleaseDC(hOwner, hdc);
}
FontProcessor::operator LOGFONT ()
{
	return logFont;
}
void FontProcessor::operator ()()
{
	LOGFONT lf;
	if (bLoaded)
		::memcpy(&lf, &logFont, sizeof(LOGFONT));

	CHOOSEFONT cf;
	::memset(&cf, 0, sizeof(cf));

	cf.lStructSize = sizeof(cf);
	cf.hwndOwner = hOwner;
	cf.lpLogFont = &lf;
	cf.Flags = CF_TTONLY | CF_EFFECTS | CF_SCREENFONTS;

	if (::ChooseFont(&cf))
	{
		::memcpy(&logFont, &lf, sizeof(cf));
	}
}
void FontProcessor::_loadWin32()
{
	std::fstream file;
	file.open(strSaveFileName, std::ios_base::in | std::ios_base::binary);

	if (!file.good())
	{
		bLoaded = false;
		file.close();
		return;
	}

	file.read((char*)&logFont, sizeof(LOGFONT));
	bLoaded = true;

	file.close();
}
void FontProcessor::_saveWin32()
{
	std::fstream file;
	file.open(strSaveFileName, std::ios_base::out | std::ios_base::binary);

	file.write((const char*)&logFont, sizeof(LOGFONT));

	file.close();
}
void FontProcessor::_loadFreetype()
{
	FT_Error err;
	err = ::FT_New_Face(library,
						"Resource\\font\\BEBAS.ttf",
						0,
						&fontface);
}
void FontProcessor::_saveFreetype()
{
}