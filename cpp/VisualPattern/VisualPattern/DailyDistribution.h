#pragma once
#include <array>
#include <vector>
#include "IntradayDistribution.h"

using namespace std;

class DailyDistribution
{
public:
	static constexpr size_t days_count_ = 25;
// types
public:
	using DaysContainer = array<IntradayDistribution, days_count_>;

	struct SourceDay
	{
		long long open_;
		long long high_;
		long long low_;
		long long close_;
	};

	using SourceContainer = vector<SourceDay>;

// data
private:
	DaysContainer days_;
	long long current_sample_min_;
	long long current_sample_max_;

// methods
private:
	bool _filterTarget(const SourceDay& day) const; // TODO: is subject to replace with lambda
	void _resetHighLow(const SourceDay& target_day) noexcept;
	void _updateHighLow(const SourceDay& day) noexcept;
	void _normalizeHighLow(const SourceDay& target_day) noexcept;
	void _updateHighLowOfSeries(SourceContainer::const_iterator begin_i, SourceContainer::const_iterator end_i) noexcept;
	void _addToDayIntensity(size_t day_index, const SourceDay& day);
	size_t _countFilteredTargetDays(const SourceContainer& source);

public:
	DailyDistribution();
	~DailyDistribution();

	void readSeries(const SourceContainer& source);

	const DaysContainer& days() const noexcept { return days_; }
};

