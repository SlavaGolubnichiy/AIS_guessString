#pragma once

#include <stdint.h>
#include <random>
#include <ctime>
#include <chrono>



static class Random
{
	public:
	static uint64_t generate64(const uint64_t& min, const uint64_t& max);
	static uint32_t generate32(const uint32_t& min, const uint32_t& max);
	static uint8_t generate8(const uint8_t& min, const uint8_t& max);

	private:
	static inline void reseedEachNanosecond();
	static inline void reseedEachMillisecond();
	static inline void reseedEachSecond();
};


