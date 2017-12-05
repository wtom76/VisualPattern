#pragma once
#include "DailyDistribution.h"

class DistributionChart
{
	static constexpr double margin_ = 0.02;

	int left_x_ = 0;
	int bottom_y_ = 0;
	double x_step_ = 0;
	double y_step_ = 0;

	DailyDistribution& data_;

	void _init(HDC hdc);
public:
	DistributionChart(DailyDistribution& data);
	~DistributionChart();

	void draw(HDC hdc);
};
