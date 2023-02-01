#pragma once

#include <ctime>
#include <stdint.h>

#define normToRange(x, min, max)	((min) + ((x) % ((max)+1-(min))))
#define normToRange0(x, max)		((x) % ((max)+1))



static inline uint32_t getNsFromEpoch()
{
	// returns uint64_t, but we optimized
	uint32_t ns =
		std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
	return ns;
}

// Marsaglia's xorshf generator
// params in constructor and generate() have to be selected carefully
// in order to provide efficient period
class RandomXorsh
{
	private:
	uint32_t state;		// leave to be initialized with random value from memory, 
						// same initial states will produce same sequences :(	=> make it Singleton

	public:
	RandomXorsh();
	uint32_t generate32();	// xorsh32
	//uint32_t generate32(uint32_t min, uint32_t max);	// xorsh32
	uint64_t generate64();
	//uint64_t generate64(uint64_t min, uint64_t max);

};

RandomXorsh::RandomXorsh()
	:
	state(getNsFromEpoch())	// seed as nanoseconds from epoch
{
}

uint32_t RandomXorsh::generate32()
{
	// state must be initialized with non-zero value
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return state = x;
}

uint64_t RandomXorsh::generate64()
{
	uint64_t x = state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	return state = x;
}



/// tests
void test_RandomXorsh()
{
	RandomXorsh xorsh;
	for (uint32_t i = 0; i < 1024; i++)
	{
		printf("%u \n", normToRange(xorsh.generate32(), 2, 10));
	}
}
