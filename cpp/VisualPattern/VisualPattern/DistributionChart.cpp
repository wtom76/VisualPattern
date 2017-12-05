#include "stdafx.h"
#include <memory>
#include <algorithm>
#include "DistributionChart.h"

#include "gdi.h"

//---------------------------------------------------------------------------------------------------------
DistributionChart::DistributionChart(DailyDistribution& data) : data_(data)
{
}
//---------------------------------------------------------------------------------------------------------
DistributionChart::~DistributionChart()
{
}
//---------------------------------------------------------------------------------------------------------
void DistributionChart::_init(HDC hdc)
{
	RECT wnd_size;
	GetClientRect(WindowFromDC(hdc), &wnd_size);

	const int width = wnd_size.right - wnd_size.left;
	left_x_ = wnd_size.left + static_cast<int>(width * margin_);
	if (!data_.days().empty())
	{
		x_step_ = static_cast<double>(width) * (1. - 2 * margin_) / static_cast<int>(data_.days().size());
	}
	
	const int height = wnd_size.bottom - wnd_size.top;
	bottom_y_ = wnd_size.bottom - static_cast<int>(height * margin_);
	y_step_ = static_cast<double>(height) * (1. - 2 * margin_) / IntradayDistribution::dots_count_;
}
//---------------------------------------------------------------------------------------------------------
void DistributionChart::draw(HDC hdc)
{
	_init(hdc);

	std::array<std::unique_ptr<Pen>, 256> pens;
	BYTE density = 0;
	const Gdiplus::REAL x_step = static_cast<Gdiplus::REAL>(x_step_);
	std::generate(begin(pens), end(pens), [x_step, &density]()
	{
		return std::make_unique<Pen>(Color(density++, 0, 0, 0), static_cast<Gdiplus::REAL>(x_step));
	});

	Graphics graphics(hdc);

	// DEBUG
	//const int left_x = left_x_;
	//const int right_x = left_x_ + static_cast<int>(x_step_ * (data_.days().size() - 1));
	//const int bottom_y = bottom_y_;
	//const int top_y = bottom_y - static_cast<int>(y_step_ * (IntradayDistribution::dots_count_ - 1));
	//// top left
	//graphics.DrawLine(pens[255].get(), left_x, top_y + 1, left_x, top_y);
	//// top right
	//graphics.DrawLine(pens[255].get(), right_x, top_y + 1, right_x, top_y);
	//// bot left
	//graphics.DrawLine(pens[255].get(), left_x, bottom_y, left_x, bottom_y - 1);
	//// bot right
	//graphics.DrawLine(pens[255].get(), right_x, bottom_y, right_x, bottom_y - 1);
	//~DEBUG

	double x = left_x_;
	for (const IntradayDistribution& day : data_.days())
	{
		double y = bottom_y_;
		for (double dot_intesity : day.dots())
		{
			graphics.DrawLine(pens[static_cast<int>(dot_intesity)].get(),
				static_cast<int>(x), static_cast<int>(y), static_cast<int>(x), static_cast<int>(y - y_step_));
			y -= y_step_;
		}
		x += x_step_;
	}
}
