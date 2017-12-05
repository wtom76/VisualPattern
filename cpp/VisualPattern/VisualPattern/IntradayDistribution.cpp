#include "stdafx.h"
#include <exception>
#include "IntradayDistribution.h"

double IntradayDistribution::intensity_step_ = 0.;
long long IntradayDistribution::real_min_ = 0;
long long IntradayDistribution::real_max_ = 0;
double IntradayDistribution::dot_pos_step_ = 0;

//---------------------------------------------------------------------------------------------------------
IntradayDistribution::IntradayDistribution()
{
}
//---------------------------------------------------------------------------------------------------------
IntradayDistribution::~IntradayDistribution()
{
}
//---------------------------------------------------------------------------------------------------------
void IntradayDistribution::setIntensityStep(double val)
{
	intensity_step_ = val;
}
//---------------------------------------------------------------------------------------------------------
void IntradayDistribution::setRealMinMax(long long min, long long max)
{
	if (max + 1 - min < dots_count_)
	{
		throw exception("Too narrow min max interval for IntradayDistribution");
	}

	real_min_ = min;
	real_max_ = max;
	dot_pos_step_ = static_cast<double>(max + 1 - min) / dots_count_;
}
//---------------------------------------------------------------------------------------------------------
void IntradayDistribution::incrementDotIntensity(long long min, long long max)
{
	long long index = _dotIndexFromRealPos(min);
	const long long max_index = _dotIndexFromRealPos(max);
	for (; index <= max_index; ++index)
	{
		dots_[index] += intensity_step_;
	}
}
