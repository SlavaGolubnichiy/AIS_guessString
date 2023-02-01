#pragma once

#include <vector>

// Solution = person
// Solutions after crossover = children
typedef std::string	Solution;
typedef uint32_t Fitness;
struct { Solution solution; Fitness fitness; } typedef SolEntry;
typedef std::vector<SolEntry> SolEntriesList;
