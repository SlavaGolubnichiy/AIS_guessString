# AIS_guessString
Artificial Immune System for guessing a long string when having data on the success of each iteration

## Description
AIS 1-iteration's algorythm:
1. select M best
2. crossover
3. mutate

2. Crossover
 void crossover(sol1, sol2, children)
 	crossoverPoint = rand(1, min(sol1, sol2)-1);
		point =	|<   v	  >|	< range >
		sol1	|..........|
		sol2	|aaaaaaaaaa|
		child1	|....aaaaaa|
		child2	|aaaa......|
	std::string child1, child2 = sol1
	child1[0 : point] = sol2[point : size-1]
	child2[0 : point] = sol1[0 : point]

## Demo
AIS tries to guess a string "Artificial Immune System"
![Result screenshot](https://raw.githubusercontent.com/SlavaGolubnichiy/AIS_guessString/master/screenshots/AIS_guesses Artificial Immune System.png?raw=true "AIS tries to guess a string Artificial Immune System")
