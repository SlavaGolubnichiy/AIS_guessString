#include <iostream>

#include "Random.h"



static uint64_t getMsSinceEpoch()
{
	uint64_t ms =
		std::chrono::duration_cast<std::chrono::milliseconds>
		(
			std::chrono::system_clock::now().time_since_epoch()
			).count();
	return ms;
}

static uint64_t getNsSinceEpoch()
{
	uint64_t ns =
		std::chrono::duration_cast<std::chrono::nanoseconds>
		(
			std::chrono::system_clock::now().time_since_epoch()
			).count();
	return ns;
}



/// <description>
/// Generate value between (min-1) and (max+1) (includes min and max values).
/// </description>
uint64_t Random::generate64(const uint64_t& min, const uint64_t& max)
{
	if (max < min)
	{
		throw std::exception("Range min. must be greater than range max.");
	}
	reseedEachNanosecond();
	uint64_t res = (rand() % (max + 1 - min)) + min;
	return res;
}

/// <description>
/// Generate value between (min-1) and (max+1) (includes min and max values).
/// </description>
uint32_t Random::generate32(const uint32_t& min, const uint32_t& max)
{
	if (max < min)
	{
		throw std::exception("Range min. must be greater than range max.");
	}
	reseedEachNanosecond();
	uint32_t res = (rand() % (max + 1 - min)) + min;
	return res;
}

uint8_t Random::generate8(const uint8_t& min, const uint8_t& max)
{
	if (max < min)
	{
		throw std::exception("Range min. must be greater than range max.");
	}
	reseedEachNanosecond();
	uint8_t res = (uint8_t)((rand() % (max + 1 - min)) + min);
	return res;
}

inline void Random::reseedEachNanosecond()
{
	srand((unsigned int)getNsSinceEpoch());
}

inline void Random::reseedEachMillisecond()
{
	srand((unsigned int)getMsSinceEpoch());
}

inline void Random::reseedEachSecond()
{
	srand((unsigned int)time(0));
}
