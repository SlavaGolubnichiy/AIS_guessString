#pragma once

#include <vector>
#include "service/Random.h"

#include "service/RandomXorsh.h"

#include "evolve/definitions.h"
#include "evolve/optimization.h"



// parallelable
void generateRandomString(Solution& strDest, uint32_t requiredSize, uint8_t charRangeMin, uint8_t charRangeMax)
{
	if (charRangeMax <= charRangeMin)
	{
		throw std::logic_error("invalid range: maximum <= minimum");
	}

	RandomXorsh rand;
	strDest.clear();
	for (uint32_t i = 0; i < requiredSize; i++)
	{
		for (uint32_t d = 0; d < UINT16_MAX * 2; d++);		// delay
		strDest.append(1, (uint8_t)(normToRange(rand.generate32(), charRangeMin, charRangeMax)));
	}
}

// parallelable
Fitness calcFitness(const Solution& strAnswer, const Solution& strSolution)
{
	if (strAnswer.size() != strSolution.size())
	{
		throw std::logic_error("Correct answer and possible solution must be the same size");
	}
	uint32_t commonSize = std::max(strAnswer.size(), strSolution.size());
	Fitness fitnessVal = 0;
	for (uint32_t i = 0; i < commonSize; i++)
	{
		fitnessVal += !((bool)(strAnswer[i] ^ strSolution[i]));		// if xor = 0 (characters are equal) => fitnessVal +1
	}
	return fitnessVal;
}

struct { uint32_t index = 0; Fitness fitness; } typedef TopEntry;

void sortDescByFitness(std::vector<TopEntry>& topList)
{
	TopEntry temp = { 0, 0 };
	bool isSorted = 0;
	for (uint32_t walk = 0; walk < topList.size() && !isSorted; walk++)
	{
		isSorted = 1;
		for (uint32_t i = 0; i < topList.size() - 1; i++)
		{
			if (topList[i].fitness < topList[i + 1].fitness)	// max first, min last, <= is must have (for topList)
			{
				temp = topList[i];
				topList[i] = topList[i + 1];
				topList[i + 1] = temp;
				isSorted = 0;
			}
		}
	}
	#if DEBUG_SORT_BY_FITNESS
		__debugbreak();
		// check sorting of topList container using debug tools
	#endif
}

/* not used */
void getNMostFit(const SolEntriesList& allSrc, SolEntriesList& bestDest, const uint32_t& bestNum)
{
	std::vector<TopEntry> topList;
	for (uint32_t i = 0; i < bestNum; i++)
	{
		topList.push_back({ 0, 0 });
	}

	// find indexes of solutions with max fitness in a sorted way 
	uint32_t topListLastIndex = topList.size() - 1;
	uint32_t allSrcSize = allSrc.size();
	for (uint32_t i = 0; i < allSrcSize; i++)
	{
		// if fitness >= topList.minFitness
		// priority to youngsters (> for elders)
		if (allSrc[i].fitness >= topList[topListLastIndex].fitness)
		{
			topList[topListLastIndex].index = i;
			topList[topListLastIndex].fitness = allSrc[i].fitness;
			sortDescByFitness(topList);
		}
	}
	
	// copy corresponding solutions to bestDest
	SolEntry buf;
	uint32_t indexInAllSrc = 0;
	uint32_t topListSize = topList.size();
	for (uint32_t i = 0; i < topListSize; i++)
	{
		indexInAllSrc = topList[i].index;
		buf = { allSrc[indexInAllSrc].solution, allSrc[indexInAllSrc].fitness };
		bestDest.push_back(buf);
	}
}

// parallelable
void crossover(const Solution& sol1, const Solution& sol2, Solution& child1Dest, Solution& child2Dest)
{
	RandomXorsh rand;
	uint32_t commonSolsSize = sol1.size();
	if (commonSolsSize != sol2.size())
	{
		throw std::logic_error("Both crossovered solutions must have same size");
	}

	//uint32_t crossoverIndex = (uint32_t)Random::generate32(0, commonSolsSize - 1);
	uint32_t crossoverIndex = normToRange(rand.generate32(), 0, commonSolsSize - 1);
	child1Dest = sol1.substr(0, crossoverIndex) + sol2.substr(crossoverIndex, sol2.size() - crossoverIndex);	// substr(offset, count)
	child2Dest = sol2.substr(0, crossoverIndex) + sol1.substr(crossoverIndex, sol1.size() - crossoverIndex);	// substr(offset, count)
}

void mutateChars(Solution& sol, uint8_t mutateRangeMin, uint8_t mutateRangeMax)
{
	RandomXorsh rand;
	uint32_t numberOfSymbolsToMutate = 4;
	for (uint32_t i = 0; i < numberOfSymbolsToMutate; i++)
	{
		uint32_t index = normToRange0(rand.generate32(), sol.size() - 1);
		sol[index] = (uint8_t)normToRange(rand.generate32(), mutateRangeMin, mutateRangeMax);
	}
}

void mutateSol(Solution& sol, char mutateRangeMin, char mutateRangeMax)
{
	generateRandomString(sol, sol.size(), mutateRangeMin, mutateRangeMax);
}