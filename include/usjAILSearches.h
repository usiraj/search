/*
 * usjAILSearches.h
 *
 *  Created on: Jan 19, 2013
 *      Author: usama
 */

#ifndef USJAILSEARCHES_H_
#define USJAILSEARCHES_H_

#include "usjAILHead.h"

namespace usjAIL{
	class Search{
		/*
		 * Result stack empty means solution not found
		 * Result stack contains node with state "FAILED" if solution failed
		 */
	public:
		Search();
		virtual ~Search();
		Stack SEARCH(const Problem &problem,void *atr1 = 0);
		//Blind search methods
		Stack TreeSearch(const Problem &problem);	// does not address loopy paths except going back to parent
		Stack GraphSearch(const Problem &problem);
		Stack BreadthFirstSearch(const Problem &problem);
		Stack DepthFirstSerach(const Problem &problem);
		Stack UniformCostSearch(const Problem &problem);
		Stack DepthLimitedSearch(const Problem &problem, const int limit);
		Stack IterativeDeepeningSearch(const Problem &problem);
		//Heuristic Search methods
		Stack GreedySearch(const Problem &problem);
		Stack AStarSearch(const Problem &problem);
	private:
		// utility functions
		Stack BuildSolution(const Node *answer) const;
		int Depth(const Node *node) const;
		Stack GeneralGraphSearch(const Problem &problem,const Stack::StackType type) const;
	};
}

#endif /* USJAILSEARCHES_H_ */
