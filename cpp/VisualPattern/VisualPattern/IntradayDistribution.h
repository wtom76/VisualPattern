#pragma once
#include <array>

using namespace std;

class IntradayDistribution
{
public:
	static constexpr size_t dots_count_ = 1024;

// types
public:
	using DotContainer = array<double, dots_count_>;

// data
private:
	static long long real_min_;
	static long long real_max_;
	static double dot_pos_step_;
	static double intensity_step_;
	DotContainer dots_ = { 0 };

// methods
	inline size_t _dotIndexFromRealPos(long long pos) const
	{
		return static_cast<size_t>((pos - real_min_) / dot_pos_step_);
	}

public:
	IntradayDistribution();
	~IntradayDistribution();

	static void setIntensityStep(double val);
	static void setRealMinMax(long long min, long long max);

	void incrementDotIntensity(long long min, long long max);
	inline const DotContainer& dots() const noexcept { return dots_; }
};

