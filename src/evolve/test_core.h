#pragma once

#include <vector>
#include "evolve/core.h"

static const char charRangeMin = 'A';
static const char charRangeMax = 'z';

/// test optimization
void test_sortDescByFitness()
{
	std::vector<TopEntry> topList =
	{
		{0, 1},
		{1, 6},
		{2, 9},
		{3, 1},
		{4, 2},
		{5, 5},
		{6, 2},
		{7, 0},
		{8, 8},
		{9, 4}
	};
	sortDescByFitness(topList);
}

void test_generateRandomStr(uint32_t strSize, uint32_t testsNum)
{
	std::string a;
	for (uint32_t i = 0; i < testsNum; i++)
	{
		generateRandomString(a, strSize, charRangeMin, charRangeMax);
		printf("%s \n", a.c_str());
	}
}

void test_calcFitness()
{
	const Solution answer = "abcdefghi";
	const Solution sol_ex = "abcd";
	const std::vector<Solution> sols =
	{
		"abcde-ghi",
		"abbbbfghi",
		"ihgfedcba"
	};

	try
	{
		printf("answer = %s, sol_ex = %s, fitness = %u \n", answer.c_str(), sol_ex.c_str(), calcFitness(answer, sol_ex));
	}
	catch (const std::exception&)
	{
		printf("answer = %s, sol_ex = %s, fitnessCalc() returned an exception as expected \n", answer.c_str(), sol_ex.c_str());
	}

	for (uint8_t i = 0; i < sols.size(); i++)
	{
		printf("answer = %s, sol1 = %s, fitness = %u \n", answer.c_str(), sols[i].c_str(), calcFitness(answer, sols[i]));
	}

}

/* not used */
void test_getNMostFit(uint32_t randomSolutionsToGenerate)
{
	std::string answer = "This is orange fox";

	SolEntriesList allSols;
	SolEntriesList bestSols;

	Solution buf;
	for (uint32_t i = 0; i < randomSolutionsToGenerate; i++)
	{
		generateRandomString(buf, answer.size(), charRangeMin, charRangeMax);							// initiale population
		SolEntry entry = { buf, calcFitness(answer, buf) };	// calc Fitness
		allSols.push_back(entry);
	}

	getNMostFit(allSols, bestSols, randomSolutionsToGenerate / 2);	// select	[under test]

	printf("answer = %s \n", answer.c_str());
	printf("best solutions: \n");
	for (uint32_t i = 0; i < bestSols.size(); i++)
	{
		printf("sol %u = %s, fitness = %u \n", i, bestSols[i].solution.c_str(), bestSols[i].fitness);
	}

}

void test_crossover(uint32_t testsNum)
{
	std::string s1, s2;
	std::string cros1, cros2;
	for (uint32_t i = 0; i < testsNum; i++)
	{
		generateRandomString(s1, 10, charRangeMin, charRangeMax);
		generateRandomString(s2, 10, charRangeMin, charRangeMax);
		crossover(s1, s2, cros1, cros2);

		printf("i = %u \n", i);
		printf("str1  = %s \n", s1.c_str());
		printf("str2  = %s \n", s2.c_str());
		printf("cros1 = %s \n", cros1.c_str());
		printf("cros2 = %s \n", cros2.c_str());
	}

}

void test_mutateChars(uint32_t testsNum)
{
	std::string str;
	for (uint32_t i = 0; i < testsNum; i++)
	{
		generateRandomString(str, 20, 'A', 'z');
		printf("i = %u \n", i);
		printf("str  = %s \n", str.c_str());
		mutateChars(str, charRangeMin, charRangeMax);
		printf("strm = %s \n", str.c_str());
	}
}
