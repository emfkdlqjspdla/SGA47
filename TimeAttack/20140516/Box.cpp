#include "Box.h"

int Box::gen_id = 0;

Box::Box()
: color(RGB(100,100,100))
, rcDraw(0, 0, 100, 50)
, bSelect(false)
, hOwner(NULL)
, id(gen_id++)
{
}
Box::~Box()
{
}
void Box::Attach(HWND hWnd)
{
	hOwner = hWnd;
}
void Box::Input(DWORD tick)
{
	Point ptMouse = InputDevice.pos();
	ptMouse = ptMouse.ToClient(hOwner);

	if (::PtInRect(&rcDraw, ptMouse))
	{
		bSelect = true;
	}
	else
	{
		bSelect = false;
	}
}
void Box::Update(DWORD tick)
{
	if (bSelect)
	{
		// TODO
	}
}
void Box::Draw(HDC hdc)
{
	HBRUSH hBrush = ::CreateSolidBrush(color);
	HBRUSH hOldBrush = ::Select(hdc, hBrush);

	HPEN hPen = ::CreatePen(PS_SOLID, 1, color);
	HPEN hOldPen = ::Select(hdc, hPen);

	::SetBkMode(hdc, TRANSPARENT);
	::Rectangle(hdc, rcDraw.left, rcDraw.top,
		rcDraw.right, rcDraw.bottom);

	std::wostringstream oss;
	oss << id;
	::DrawText(hdc, oss.str().c_str(), -1, &rcDraw, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	::Select(hdc, hOldPen);
	::DeleteObject(hPen);

	::Select(hdc, hOldBrush);
	::DeleteObject(hBrush);
}
void Box::SetDrawRect(const Rect& rc)
{
	rcDraw = rc;
}
void Box::SetColor(const COLORREF clr)
{
	color = clr;
}
Rect Box::GetDrawRect() const
{
	return rcDraw;
}
COLORREF Box::GetColor() const
{
	return color;
}
bool Box::IsSelect() const
{
	return bSelect;
}