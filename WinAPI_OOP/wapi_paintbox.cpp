#include "wapi_paintbox.h"

constexpr const LPARAM mycall = 1337;

PaintBox::PaintBox(HINSTANCE hInstance, WORD id, Control* parent) : NonTopControl(hInstance, id, parent)
{
	if (!Control::IsRegistered(ClassName())) throw std::logic_error("PaintBox is not registered");

	InitComponent(parent);
}

void PaintBox::SetPen(PenStyle style, int width, Color color)
{
	HPEN pen = CreatePen(style, width, RGB(color.r, color.g, color.b));
	paintEvents.push_back(std::make_shared<UpdatePen>(pen));
}

LRESULT PaintBox::OnPaint(HDC& hdc, PAINTSTRUCT& ps, WPARAM wp, LPARAM lp)
{
	if (lp == mycall)
	{
		for (auto const& pp : paintEvents)
		{
			pp->Handle(hdc);
		}
		ValidateRect(hwnd, nullptr);
	}
	return 0;
}

DWORD PaintBox::DefaultStyle() const
{
	return NonTopControl::DefaultStyle() | WS_BORDER;
}