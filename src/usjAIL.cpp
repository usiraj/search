//============================================================================
// Name        : usjAIL.cpp
// Author      : Usama Siraj
// Version     :
// Copyright   : AIResearch
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "usjAILHead.h"
#include "usjAILSearches.h"
#include "usjMapProblem.h"
using namespace std;
using namespace usjAIL;

int main() {
	// generate a problem
	cout << " Testing Search" << endl;
	Stack result;
	MapProblem romania("Romania.map");
	Search searcher;
	State istate = romania.getStateWithName("Timisoara");
	State fstate = romania.getStateWithName("Bucharest");
	romania.defineInitialState(istate);
	romania.defineGoalState(fstate);
	cout << "Searchig with initial state " << istate.getStateName() <<
			" and goal state " << fstate.getStateName() << endl;
	// do serach
	result = searcher.GreedySearch(romania);
	printResult(result);
	return 0;
}
