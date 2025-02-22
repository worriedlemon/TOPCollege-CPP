#pragma once

#ifndef WINAPI_PAINTBOX_H
#define WINAPI_PAINTBOX_H

#include "wapi_nontopcontrol.h"
#include <list>
#include <vector>

extern const LPARAM mycall;

struct Color
{
	int r, g, b;
};

enum Figure : long
{
	LINE = 0,
	RECTANGLE = 1,
	ELLIPSE = 2,
	ARC = 3
};

class PaintEvent
{
protected:
	PaintEvent() = default;
	virtual ~PaintEvent() = default;
public:
	virtual void Handle(HDC& hdc) = 0;
};

struct PaintText : PaintEvent
{
	int x;
	int y;
	std::wstring text;

	PaintText(int x, int y, std::wstring const& str)
		: PaintEvent()
		, x(x), y(y), text(str) {}

	void Handle(HDC& hdc) final
	{
		TextOutW(hdc, x, y, text.c_str(), text.size());
	}
};

template <Figure f>
struct PaintFigure : PaintEvent
{
	std::vector<int> x;

	PaintFigure(std::vector<int> const & coords)
		: PaintEvent()
		, x(coords)
	{
		if (coords.size() != 4) throw std::invalid_argument("Size should be 4");
	}

	void Handle(HDC& hdc) final
	{
		switch (f)
		{
		case Figure::LINE:
			MoveToEx(hdc, x[0], x[1], nullptr);
			LineTo(hdc, x[2], x[3]);
			break;
		case Figure::RECTANGLE:
			Rectangle(hdc, x[0], x[1], x[2], x[3]);
			break;
		case Figure::ELLIPSE:
			Ellipse(hdc, x[0], x[1], x[2], x[3]);
			break;
		}
	}
};

template <>
struct PaintFigure<Figure::ARC> : PaintEvent
{
	std::vector<int> x;

	PaintFigure(std::vector<int> const & coords)
		: PaintEvent()
		, x(coords)
	{
		if (coords.size() != 8) throw std::invalid_argument("Size should be 8");
	}

	void Handle(HDC& hdc) final
	{
		Arc(hdc, x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]);
	}
};

struct UpdatePen : PaintEvent
{
	HPEN pen;

	UpdatePen(HPEN pen) : PaintEvent(), pen(pen) {}
	~UpdatePen() { DeleteObject(pen); }

	void Handle(HDC& hdc) final
	{
		SelectObject(hdc, pen);
	}
};

class PaintBox : public NonTopControl
{
	std::list<std::shared_ptr<PaintEvent>> paintEvents;

public:

	enum PenStyle
	{
		Solid = PS_SOLID,
		Dashed = PS_DASH,
		Dotted = PS_DOT,
		DashDot = PS_DASHDOT,
		DashDotDot = PS_DASHDOTDOT,
		None = PS_NULL
	};

	PaintBox(HINSTANCE hInstance, WORD id, Control* parent);
	~PaintBox() = default;

	void SetPen(PenStyle style, int width, Color color);

	template <Figure f>
	void Draw(std::vector<int> const & coords)
	{
		std::shared_ptr<PaintEvent> ev = std::make_shared<PaintFigure<f>>(coords);
		paintEvents.push_back(ev);
	}

	void DrawString(int x, int y, std::wstring text)
	{
		std::shared_ptr<PaintEvent> ev = std::make_shared<PaintText>(x, y, text);
		paintEvents.push_back(ev);
	}

	inline void Update()
	{
		PostMessageW(hwnd, WM_PAINT, 0, mycall);
	}

	inline void Clear()
	{
		paintEvents.clear();
	}

protected:
	LRESULT OnPaint(HDC& hdc, PAINTSTRUCT& ps, WPARAM wp, LPARAM) override;

	DWORD DefaultStyle() const override;

	DECLARE_CLASS_NAME(PaintBox)
};

#endif // WINAPI_PAINTBOX_H