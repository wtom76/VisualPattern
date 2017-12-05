#include "stdafx.h"
#include "DailyDistribution.h"

//---------------------------------------------------------------------------------------------------------
DailyDistribution::DailyDistribution()
{
}
//---------------------------------------------------------------------------------------------------------
DailyDistribution::~DailyDistribution()
{
}
//---------------------------------------------------------------------------------------------------------
bool DailyDistribution::_filterTarget(const SourceDay& day) const
{
	// take 1% growth
	return static_cast<double>(day.close_ - day.open_) / static_cast<double>(day.open_) > 0.005;
}
//---------------------------------------------------------------------------------------------------------
void DailyDistribution::_resetHighLow(const SourceDay& target_day) noexcept
{
	current_sample_min_ = std::min(target_day.open_, target_day.close_);
	current_sample_max_ = std::max(target_day.open_, target_day.close_);
}
//---------------------------------------------------------------------------------------------------------
void DailyDistribution::_updateHighLow(const SourceDay& target_day) noexcept
{
	if (current_sample_min_ > min(target_day.open_, target_day.close_))
	{
		current_sample_min_ = min(target_day.open_, target_day.close_);
	}
	if (current_sample_max_ < max(target_day.open_, target_day.close_))
	{
		current_sample_max_ = max(target_day.open_, target_day.close_);
	}
}
//---------------------------------------------------------------------------------------------------------
void DailyDistribution::_normalizeHighLow(const SourceDay& target_day) noexcept
{
	const long long open_high_distance = current_sample_max_ - target_day.open_;
	const long long open_low_distance = target_day.open_ - current_sample_min_;
	if (open_high_distance < open_low_distance)
	{
		current_sample_max_ = target_day.open_ + open_low_distance;
	}
	else if(open_high_distance > open_low_distance)
	{
		current_sample_min_ = target_day.open_ - open_high_distance;
	}
}
//---------------------------------------------------------------------------------------------------------
void DailyDistribution::_updateHighLowOfSeries(
	SourceContainer::const_iterator begin_i,
	SourceContainer::const_iterator end_i) noexcept
{
	const SourceDay& target_day = *reverse_iterator<SourceContainer::const_iterator>(end_i);
	_resetHighLow(target_day);
	for_each(begin_i, end_i, [this](const SourceDay& day) { _updateHighLow(day); });
	_normalizeHighLow(target_day);

	IntradayDistribution::setRealMinMax(current_sample_min_, current_sample_max_);
}
//---------------------------------------------------------------------------------------------------------
void DailyDistribution::_addToDayIntensity(size_t day_index, const SourceDay& day)
{
	days_[day_index].incrementDotIntensity(min(day.open_, day.close_), max(day.open_, day.close_));
}
//---------------------------------------------------------------------------------------------------------
size_t DailyDistribution::_countFilteredTargetDays(const SourceContainer& source)
{
	size_t result = 0;
	SourceContainer::const_iterator series_begin_i = cbegin(source); // current series begin
	const SourceContainer::const_iterator global_end_i = cend(source); // global data end
	const long long target_day_count = source.size() - days_count_ + 1; // number of full series available
	if (target_day_count < 1)
	{
		return 0;
	}

	SourceContainer::const_iterator series_end_i = series_begin_i;
	advance(series_end_i, days_count_);

	while (series_end_i != global_end_i)
	{
		if (_filterTarget(*reverse_iterator<SourceContainer::const_iterator>(series_end_i)))
		{
			++result;
		}
		++series_begin_i;
		++series_end_i;
	}
	return result;
}
//---------------------------------------------------------------------------------------------------------
void DailyDistribution::readSeries(const SourceContainer& source)
{
	const size_t filtered_target_count = _countFilteredTargetDays(source);
	if (filtered_target_count < 1)
	{
		return;
	}
	SourceContainer::const_iterator series_begin_i = cbegin(source); // current series begin
	const SourceContainer::const_iterator global_end_i = cend(source); // global data end
	IntradayDistribution::setIntensityStep(255. / static_cast<double>(filtered_target_count));

	SourceContainer::const_iterator series_end_i = series_begin_i;
	advance(series_end_i, days_count_);

	while (series_end_i != global_end_i)
	{
		if (_filterTarget(*reverse_iterator<SourceContainer::const_iterator>(series_end_i)))
		{
			_updateHighLowOfSeries(series_begin_i, series_end_i);

			size_t day_index = 0;
			for_each(series_begin_i, series_end_i, [this, &day_index](const SourceDay& day) { _addToDayIntensity(day_index++, day); });
		}
		++series_begin_i;
		++series_end_i;
	}
}
