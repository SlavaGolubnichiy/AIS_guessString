#pragma once

#include "evolve/core.h"

#define DO_DEBUG 0

// if solutions are stored in a sorted way => 
// therefore there is no need in getNMostFit() - you can 
// just grab few first - and they'll be most fit
void sortBubbleDescByFitness(SolEntriesList& list)
{
	SolEntry temp;

	for (uint32_t iter = 0; iter < list.size(); iter++)
	{
		for (uint32_t i = 0; i < list.size() - 1; i++)
		{
			if (list[i].fitness < list[i + 1].fitness)
			{
				temp = list[i];
				list[i] = list[i + 1];
				list[i + 1] = temp;
			}
		}
	}
}

void sortShakerStopDescByFitness(SolEntriesList& list)
{
	uint32_t listSize = list.size();
	SolEntry temp;
	bool isSorted = 0;

	for (uint32_t k = 0; k < (listSize / 2) && !isSorted; k++)
	{
		uint32_t i = 0;
		isSorted = 1;

		for (i = 0; i < listSize - 1; i++)
		{
			if (list[i].fitness < list[i + 1].fitness)
			{
				temp = list[i + 1];
				list[i + 1] = list[i];
				list[i] = temp;

				isSorted = 0;
			}
		}

		for (; i > 0; i--)
		{
			if (list[i - 1].fitness < list[i].fitness)
			{
				temp = list[i];
				list[i] = list[i - 1];
				list[i - 1] = temp;

				isSorted = 0;
			}
		}
	}
}



#if DO_DEBUG

// helps to speed up testing of sort() functions

void sortBubbleDesc(std::vector<uint32_t>& list)
{
	uint32_t temp;

	for (uint32_t iter = 0; iter < list.size(); iter++)
	{
		for (uint32_t i = 0; i < list.size() - 1; i++)
		{
			if (list[i] < list[i + 1])
			{
				temp = list[i];
				list[i] = list[i + 1];
				list[i + 1] = temp;
			}
		}
	}

}

void test_sortBubbleDesc()
{
	std::vector<uint32_t> nums = { 0,6,5,3,6,8,9,5,1,5,4,3,2,4,9,8,4,6,8,7 };
	printf("init_nums = ");
	for (uint32_t i = 0; i < nums.size(); i++)
	{
		printf("%u ", nums[i]);
	}
	sortBubbleDesc(nums);
	printf("sort_nums = ");
	for (uint32_t i = 0; i < nums.size(); i++)
	{
		printf("%u ", nums[i]);
	}
}


void sortShakerStopDesc(std::vector<uint32_t>& list)
{
	uint32_t listSize = list.size();
	uint32_t temp;
	bool isSorted = 0;

	for (uint32_t k = 0; k < (listSize / 2) && !isSorted; k++)
	{
		uint32_t i = 0;
		isSorted = 1;

		for (i = 0; i < listSize - 1; i++)
		{
			if (list[i] < list[i + 1])		// change to > for Ascending
			{
				temp = list[i + 1];
				list[i + 1] = list[i];
				list[i] = temp;

				isSorted = 0;
			}
		}

		for (; i > 0; i--)
		{
			if (list[i - 1] < list[i])		// change to > for Ascending
			{
				temp = list[i];
				list[i] = list[i - 1];
				list[i - 1] = temp;

				isSorted = 0;
			}
		}
	}
}

void test_sortDescShakerStop()
{
	std::vector<uint32_t> nums = { 0,6,5,3,6,8,9,5,1,5,4,3,2,4,9,8,4,6,8,7 };
	printf("nums = ");
	for (uint32_t i = 0; i < nums.size(); i++)
	{
		printf("%u ", nums[i]);
	}
	sortShakerStopDesc(nums);
	printf("nums = ");
	for (uint32_t i = 0; i < nums.size(); i++)
	{
		printf("%u ", nums[i]);
	}
}

#endif

#undef DO_DEBUG
