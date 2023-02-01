#include <iostream>
#include <vector>

#include "service/Random.h"
#include "evolve/core.h"
#define DO_DEBUG 1
#if DO_DEBUG
	#include "evolve/test_core.h"
#endif

#include "service/Timer.h"







namespace App
{
	const char answerSymbolsRange_min = 0x20;		// [SPACE]
	const char answerSymbolsRange_max = 0x7E;		// '~'

	const uint32_t startSolutionsNumber = 40;
	const uint32_t oneMutationPerNumber = 2;		// 1/1 = 100%, 1/2 = 50%, 1/5 = 20%
	SolEntriesList population;			// registry with all the found solutions (population)

};

void print(const char* pre, const std::vector<uint32_t>& vec, const char* post)
{
	printf("%s", pre);
	for (uint32_t i = 0; i < vec.size(); i++)
	{
		printf("%u ", vec[i]);
	}
	printf("%s", post);
}

void print(SolEntry entry)
{
	printf("str = %s, fitness = %u \n", entry.solution.c_str(), entry.fitness);
}

uint32_t toLowerEven(const uint32_t& val)
{
	if (!val || val == 1)
	{
		throw std::invalid_argument("val = 0 is not acceptable");
	}
	return 
		((val % 2) * (val-1)) |		// if (odd) val-1;
		(!(val % 2) * (val));		// else val;
}

// parallelable
inline void getVecCut(const std::vector<uint32_t>& srcVec, const uint32_t& indexFrom, const uint32_t& indexTo, std::vector<uint32_t>& dstVec)
{
	if (srcVec.size() <= indexFrom || srcVec.size() <= indexTo)
	{
		throw std::out_of_range("indexFrom or indexTo is out of srcVec's index range");
	}

	dstVec.clear();
	if (indexFrom < indexTo)		// direct way
	{
		for (uint32_t si = indexFrom; si < indexTo + 1; si++)
		{
			dstVec.push_back(srcVec[si]);
		}
	}
	//else if (indexTo < indexFrom)		// opposite way (not implemented)
	//{
	//	for (uint32_t si = indexFrom; (si+1) > indexTo; si++)
	//	{
	//		dstVec.push_back(srcVec[si]);
	//	}
	//}
	else
	{
		throw std::logic_error("combination of indexFrom and indexTo is invalid");
	}
}

void test_getVecCut()
{
	std::vector<uint32_t> vec1 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	{
		std::vector<uint32_t> cut1, cut2, cut3, cut4;
		getVecCut(vec1, 0, 5, cut1);
		getVecCut(vec1, 5, 11, cut2);
		getVecCut(vec1, 12, 19, cut3);

		printf("\t0 1 2 3 4 5 6 7 8 9  10 11 12 13 14 15 16 17 18 19\n");
		print("vec1 = \t", vec1, "\n");
		print("cut1 [0:5] = ", cut1, "\n");
		print("cut2 [5:11] = ", cut2, "\n");
		print("cut3 [12:19] = ", cut3, "\n");
	}

	#if 0
	{
		std::vector<uint32_t> rcut1, rcut2, rcut3, rcut4;
		getVecCut(vec1, 5, 0, rcut1);
		getVecCut(vec1, 11, 5, rcut2);
		getVecCut(vec1, 19, 12, rcut3);

		printf("\t0 1 2 3 4 5 6 7 8 9  10 11 12 13 14 15 16 17 18 19\n");
		print("vec1 = \t", vec1, "\n");
		print("rcut1 [0:5] = ", rcut1, "\n");
		print("rcut2 [5:11] = ", rcut2, "\n");
		print("rcut3 [12:19] = ", rcut3, "\n");
	}
	#endif

}

// parallelable
inline void getVecCut(const SolEntriesList& srcVec, const uint32_t& indexFrom, const uint32_t& indexTo, SolEntriesList& dstVec)
{
	if (srcVec.size() <= indexFrom || srcVec.size() <= indexTo)
	{
		throw std::out_of_range("indexFrom or indexTo is out of srcVec's index range");
	}

	dstVec.clear();
	if (indexFrom < indexTo)		// direct way
	{
		for (uint32_t si = indexFrom; si < indexTo + 1; si++)
		{
			dstVec.push_back(srcVec[si]);
		}
	}
	//else if (indexTo < indexFrom)		// opposite way (not implemented)
	//{
	//	for (uint32_t si = indexFrom; (si+1) > indexTo; si++)
	//	{
	//		dstVec.push_back(srcVec[si]);
	//	}
	//}
	else
	{
		throw std::logic_error("combination of indexFrom and indexTo is invalid");
	}
}




void evolve(const std::string& answer, const uint32_t& generationsLimit, SolEntry& evolveResultDest)
{
	#define DO_LOG_NEW_SOL 0
	#define DO_LOG_BEST_CHANGE_SOL 1

	// generate App::initialNum number of solutions -> to SolEntriesList
	// from SolEntriesList -> calc their fitnesses -> to SolEntriesList
	// from SolEntriesList -> getNMostFit() -> to SolEntriesListBest
	// from SolEntriesListBest -> crossover[1-2, 3-4, 5-6...] -> append child[0,1] to SolEntriesList
	// rand % App::mutationPerNumber, if 0 -> mutate(child[i]) -> it edits SolEntriesList

	// wrap previous steps into loop which stops when
	//		getNMostFit()'s max fitness == answer.size() || iterations = App::iterationsLim
	//			// save getNMostFit() because if loop ends - this will suggest the best solution

	Timer timer;
	#if DO_LOG_BEST_CHANGE_SOL
	timer.start();
	#endif

	/// initiate population
	SolEntriesList population;
	std::string randSol;
	for (uint32_t i = 0; i < App::startSolutionsNumber; i++)
	{
		generateRandomString(randSol, answer.size(), App::answerSymbolsRange_min, App::answerSymbolsRange_max);
		uint32_t fitness = calcFitness(answer, randSol);
		SolEntry solEntry = { randSol, fitness };
		population.push_back(solEntry);
		#if DO_LOG
			printf("gen 0: "); print(solEntry);
		#endif
	}
	sortShakerStopDescByFitness(population);


	double timeSpent = 0;
	RandomXorsh rand;

	uint32_t numOfMostFitToSelect;
	uint32_t numOfDeleted;
	SolEntriesList bestOfGen;
	Solution child1, child2;
	uint32_t child1Fitness, child2Fitness;
	Fitness prevBestFitness = 0;
	Fitness worstFitness;
	for (uint32_t gen = 1; gen < generationsLimit && population[0].fitness < answer.size(); gen++)
	{
		bestOfGen.clear();
		numOfMostFitToSelect = toLowerEven((uint32_t)((float)population.size() / 10.0 * 2.5));	// +25 %, equals the num of new children after crossover
		SolEntriesList genChildren(numOfMostFitToSelect);		// to store best children
		numOfDeleted = (uint32_t)((float)population.size() / 10.0 * 2);							// -20 %
		#if DO_LOG_NEW_SOL
		printf("gen %u \n", gen);
		//printf("\t population size = %u \n", population.size());
		#endif
		
		/// select best
		// best solutions are at the top [0 index] of a vector, because we keep it always sorted
		getVecCut(population, 0, numOfMostFitToSelect - 1, bestOfGen);	// num has to be even(!) (because see next step algo)
		/// besties' crossover + mutate + save to genChildren
		worstFitness = population[population.size() - 1].fitness;
		uint32_t successGraduatesCount = 0;
		uint32_t mutatesHappened = 0;
		uint32_t mutatesPossible = 0;
		for (uint32_t i = 0; i < bestOfGen.size(); i += 2)
		{
			/// crossover
			crossover(bestOfGen[i].solution, bestOfGen[i + 1].solution, child1, child2);
			/// mutation
			bool isMutation1 = !((uint8_t)normToRange0(rand.generate32(), App::oneMutationPerNumber - 1));
			bool isMutation2 = !((uint8_t)normToRange0(rand.generate32(), App::oneMutationPerNumber - 1));
			if (isMutation1)
			{
				mutateChars(child1, App::answerSymbolsRange_min, App::answerSymbolsRange_max);
			}
			if (isMutation2)
			{
				mutateChars(child2, App::answerSymbolsRange_min, App::answerSymbolsRange_max);
			}
			/// save specific genChildren to population
			child1Fitness = calcFitness(answer, child1);
			child2Fitness = calcFitness(answer, child2);
			//if (child1Fitness != worstFitness)		// better than worst (another method of adding)
			if (child1Fitness >= population[0].fitness)	// same or more fit than current best
			{
				population.push_back({ child1, calcFitness(answer, child1) });
				#if DO_LOG_NEW_SOL
					printf("gen %u: ", gen); print(genChildren[i]);
				#endif
			}
			//if (child2Fitness != worstFitness)		// better than worst (another method of adding)
			if (child2Fitness >= population[0].fitness)	// same or more fit than current best
			{
				population.push_back({ child2, calcFitness(answer, child2) });
				#if DO_LOG_NEW_SOL
					printf("gen %u: ", gen); print(genChildren[i]);
				#endif
			}
		}
		sortShakerStopDescByFitness(population);
		/// compression
		uint32_t indexLimit = population.size() - numOfDeleted;
		for (uint32_t i = population.size() - 1; (population[i].fitness > worstFitness) & (i > indexLimit) | (i > 1023); i--)	// limit max population size = 1024 (1023)
		{
			population.pop_back();
		}


		#if DO_LOG_BEST_CHANGE_SOL
		if (population[0].fitness > prevBestFitness)
		{
			timer.stop();
			timeSpent += timer.getElapsedSeconds();
			timer.start();
			printf("gen %.5u: best= \"%s\", fit=%u/%u, t+%.3f s\n", gen, population[0].solution.c_str(), population[0].fitness, answer.size(), timeSpent);
			prevBestFitness = population[0].fitness;
		}
		#endif
	}
	timer.stop();

	evolveResultDest = population[0];

	#undef DO_LOG_NEW_SOL
	#undef DO_LOG_BEST_CHANGE_SOL
}





void sortShakerStopAscByFitness(SolEntriesList& list)
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
			if (list[i].fitness > list[i + 1].fitness)
			{
				temp = list[i + 1];
				list[i + 1] = list[i];
				list[i] = temp;

				isSorted = 0;
			}
		}

		for (; i > 0; i--)
		{
			if (list[i - 1].fitness > list[i].fitness)
			{
				temp = list[i];
				list[i] = list[i - 1];
				list[i - 1] = temp;

				isSorted = 0;
			}
		}
	}
}

void consoleFlushStdin(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void consoleTakeInput(char* inputBuf, const uint32_t inputSize)
{
	if (inputBuf == nullptr)
	{
		throw std::exception("inputBuf = nullptr, so inputBuf can't be used");
	}
	for (uint32_t i = 0; i < inputSize; i++)
	{
		inputBuf[i] = '\0';
	}
	fgets(inputBuf, inputSize, stdin);	// read string
	consoleFlushStdin();				// flush stdin

	// make first occurance of '\n' terminate ('\0') the string
	for (uint32_t i = 0; i < inputSize; i++)
	{
		if (inputBuf[i] == '\n')
		{
			inputBuf[i] = '\0';
			break;
		}
	}
}


#include "service/RandomXorsh.h"

void test_stdVecConcat()
{
	Timer timer;

	std::vector<uint32_t> a = { 1, 2, 3, 4, 5, 6 };
	std::vector<uint32_t> b = { UINT32_MAX, UINT32_MAX - 1, UINT32_MAX - 2, UINT32_MAX - 3, UINT32_MAX - 4, UINT32_MAX - 5 };
	std::vector<uint32_t> c(a.size() + b.size());

	timer.start();
	uint32_t i = 0;
	for (i = 0; i < b.size(); i++)
	{
		a.push_back(b[i]);
	}
	timer.stop();
	printf("case1 time = %f ns \n", (float)timer.getElapsedSeconds());

	print("a = ", a, "\n");
	a = { 1,2,3 };

	timer.start();
	a.insert(a.end(), b.begin(), b.end());		/// concat vector a with vector b		<- ~2-3 times faster   !!!!! use it
	timer.stop();
	printf("case2 time = %f ns \n", (float)timer.getElapsedSeconds());

	print("a = ", a, "\n");
}





int main()
{
	//const uint32_t inputLenLim = 64;
	//char user_name[inputLenLim+1];
	//char user_uniName[inputLenLim+1];
	//char user_uniGroup[inputLenLim+1];
	//char user_speciality[inputLenLim+1];
	//printf("Enter your legal name [max. %u symbols]: ", inputLenLim);
	//consoleTakeInput(user_name, inputLenLim+1);
	//printf("Enter your university name [max. %u symbols]: ", inputLenLim);
	//consoleTakeInput(user_uniName, inputLenLim+1);
	//printf("Enter your university group code [max. %u symbols]: ", inputLenLim);
	//consoleTakeInput(user_uniGroup, inputLenLim+1);
	//printf("Enter your speciality [max. %u symbols]: ", inputLenLim);
	//consoleTakeInput(user_speciality, inputLenLim+1);

	//std::string answer = 
		//std::string(user_name);// + std::string(", ") + 
		//std::string(user_uniName) + std::string(", ") +
		//std::string(user_uniGroup) + std::string(", ") + 
		//std::string(user_speciality);
	
	/// test with Slava, Vlada, Artem, Jenia		// 5 symbols
	/// also with RedCuteFox, GoodParrot, ShyOctopus, GreyKitten		// 10 symbols
	// vary in [line: 525 / 526] and [line: 542]

	std::string answer = "Artificial Immune System";

	Timer tim;
	tim.start();

	SolEntry result;
	evolve(answer, UINT32_MAX, result);
	printf("str = \"%s\", fitness = %u", result.solution.c_str(), result.fitness);

	tim.stop();
	printf("time = %f s \n", (float)tim.getElapsedSeconds());




	/*
	Timer timer;

	std::vector<uint32_t> a = { 1, 4, 5, 7, 9, 10 };
	std::vector<uint32_t> b = { 1, 2, 3, 6, 7, 8};
	std::vector<uint32_t> c(a.size() + b.size());

	timer.start();
	uint32_t i = 0;
	
	timer.stop();
	printf("case1 time = %f ns \n", (float)timer.getElapsedSeconds());

	print("a = ", a, "\n");
	a = { 1,2,3 };

	timer.start();
	a.insert(a.end(), b.begin(), b.end());		/// concat vector a with vector b		<- ~2-3 times faster   !!!!! use it
	timer.stop();
	printf("case2 time = %f ns \n", (float)timer.getElapsedSeconds());

	print("a = ", a, "\n");
	*/

	printf("Program reached the end. \n");
	system("pause");
	return 0;
}









/// code trash bin
/*




// not used
void sort(uint32_t* ar, const uint32_t arSize)
{
	uint32_t temp;
	bool isSorted = 0;
	for (uint32_t wlkthr = 0; wlkthr < arSize && !isSorted; wlkthr++)
	{
		isSorted = 1;
		for (uint32_t i = 0; i < arSize - 1; i++)
		{
			if (ar[i] > ar[i + 1])
			{
				temp = ar[i];
				ar[i] = ar[i + 1];
				ar[i + 1] = temp;
				isSorted = 0;
			}
		}
	}
	#if DEBUG_SORT
	__debugbreak();
	// check sorting of ar using debug tools
	#endif

}


// not used
void test_sort()
{
	const uint32_t ar1Size = 20;
		  uint32_t ar1[ar1Size] = {6,5,7,8,4,4,65,7,5,4,5,7,65,54,326,67,6,21,56,64};
	const uint32_t ar2Size = 13;
		  uint32_t ar2[ar2Size] = { 1515,135135,1651,61237,27,27,247,247,7242872,2372,72,72,274 };
	const uint32_t ar3Size = 18;
		  uint32_t ar3[ar3Size] = { 515,135,1346,46,146,476,42,5789,234,753,2134,73,214,386,346,146,23578,24 };
	sort(ar1, ar1Size);
	sort(ar2, ar2Size);
	sort(ar3, ar3Size);
}











*/