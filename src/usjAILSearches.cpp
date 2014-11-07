/*
 * usjAILSearches.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: usama
 */
#include "usjAILSearches.h"
#include <iostream>
using namespace std;
namespace usjAIL{
	Search::Search(){

	}
	Search::~Search(){

	}
	Stack Search::SEARCH(const Problem &problem,void *atr1){
		return this->GraphSearch(problem);
	}
	//Blind search methods
	Stack Search::TreeSearch(const Problem &problem){
		Stack Frontier;
		Node *leaf;
		Frontier.setStackType(Stack::FIFO);
		Frontier.PUSH(Node(problem.getInitialState()));
		while ( !Frontier.ISEMPTY() ){
			leaf = new Node;
			*leaf = Frontier.POP();
			if ( problem.Goal_Test(*leaf) ){
				return this->BuildSolution(leaf);
			}
			// add resulting nodes from expansion to frontier
			if ( problem.getTotalActionsForState(leaf->STATE()) == 0){
				// dead end clear the leaf
				delete leaf;
			} else {
				for ( int i = 1; i <= problem.getTotalActionsForState(leaf->STATE()); ++i){
					// skip back to parent
					Node child = problem.CHILDNODE(*leaf,problem.getActionIforState(leaf->STATE(),i));
					if ( !((leaf->PARENT() != 0) && (child.STATE() == leaf->PARENT()->STATE())) ){
						Frontier.PUSH( child );
					}
				}
			}
		}
		return this->BuildSolution(0);
	}
	Stack Search::GraphSearch(const Problem &problem){
		return this->GeneralGraphSearch(problem,Stack::LIFO);
	}
	Stack Search::BreadthFirstSearch(const Problem &problem){
		Stack Frontier;
		Stack ExploredSet;
		Node *leaf;
		if ( problem.Goal_Test(Node(problem.getInitialState()))) {
			leaf = new Node;
			*leaf = Node(problem.getInitialState());
			return this->BuildSolution(leaf);
		}
		Frontier.setStackType(Stack::FIFO);
		Frontier.PUSH(Node(problem.getInitialState()));
		while ( !Frontier.ISEMPTY() ){
			leaf = new Node;
			*leaf = Frontier.POP();
			// add the leaf node to explored set
			ExploredSet.PUSH(*leaf);
			// add resulting nodes from expansion to frontier
			for ( int i = 1; i <= problem.getTotalActionsForState(leaf->STATE()); ++i){
				// skip back to parent
				Node child = problem.CHILDNODE(*leaf,problem.getActionIforState(leaf->STATE(),i));
				if ( !((leaf->PARENT() != 0) && (child.STATE() == leaf->PARENT()->STATE())) ){
					if ( !ExploredSet.DOEXIST(child.STATE()) ){
						if ( problem.Goal_Test(child) ){
							Node* ans = new Node;
							*ans = child;
							return this->BuildSolution(ans);
						} else	Frontier.PUSH( child );
					}
				}
			}
		}
		return this->BuildSolution(0);
	}
	Stack Search::DepthFirstSerach(const Problem &problem){
		return this->GeneralGraphSearch(problem,Stack::LIFO);
	}
	Stack Search::UniformCostSearch(const Problem &problem){
		return this->GeneralGraphSearch(problem,Stack::PRIORITY_PATH);
	}
	Stack Search::DepthLimitedSearch(const Problem &problem, const int limit){
		Stack Frontier;
		Stack ExploredSet;
		Node *leaf;
		bool cutoff = false;
		Frontier.setStackType(Stack::LIFO);
		Frontier.PUSH(Node(problem.getInitialState()));
		while ( !Frontier.ISEMPTY() ){
			leaf = new Node;
			*leaf = Frontier.POP();
			if ( problem.Goal_Test(*leaf) ){
				return this->BuildSolution(leaf);
			}
			// add the leaf node to explored set
			ExploredSet.PUSH(*leaf);
			// if the leaf is dead : no children delete it
			if ( problem.getTotalActionsForState(leaf->STATE()) == 0){
				delete leaf;
			} else if ( this->Depth(leaf) < limit ) {
			// add resulting nodes from expansion to frontier
			for ( int i = 1; i <= problem.getTotalActionsForState(leaf->STATE()); ++i){
				// skip back to parent
				Node child = problem.CHILDNODE(*leaf,problem.getActionIforState(leaf->STATE(),i));
				if ( !((leaf->PARENT() != 0) && (child.STATE() == leaf->PARENT()->STATE())) ){
					if ( !ExploredSet.DOEXIST(child.STATE()) ){
						Frontier.PUSH( child );
					}
				}
			}
			} else {
				cutoff = true;
			}
		}
		if ( cutoff ){
			return Stack();		// an empty stack
		}
		return this->BuildSolution(0);
	}
	Stack Search::IterativeDeepeningSearch(const Problem &problem){
		int i = 1;
		Stack solution;
		while ( solution.ISEMPTY() ){
			solution = DepthLimitedSearch(problem,i);
			++i;
		}
		return solution;
	}
	//Heuristic Search methods
	Stack Search::GreedySearch(const Problem &problem){
		return this->GeneralGraphSearch(problem,Stack::PRIORITY_EXPECTED);
	}
	Stack Search::AStarSearch(const Problem &problem){
		return this->GeneralGraphSearch(problem,Stack::PRIORITY_COMBINED);
	}
	// utility functions
	int Search::Depth(const Node *node) const{
		int depth = 1;
		if ( node == 0) return 0;
		Node* parent = node->PARENT();
		while ( parent != 0){
			++depth;
			parent = parent->PARENT();
		}
		return depth;
	}
	Stack Search::BuildSolution(const Node *answer) const{
		Stack solution;
		if ( answer == 0){
			solution.PUSH(Node("FAILED"));
			return solution;
		}
		solution.PUSH(*answer);
		Node* parent = answer->PARENT();
		delete answer;
		answer = 0;
		while ( parent != 0){
			solution.PUSH(*parent);
			Node *temp = parent;
			parent = parent->PARENT();
			delete temp;
		}
		return solution;
	}
	Stack Search::GeneralGraphSearch(const Problem &problem,const Stack::StackType type) const{
		Stack Frontier;
		Stack ExploredSet;
		Node *leaf;
		Frontier.setStackType(type);
		Frontier.PUSH(Node(problem.getInitialState()));
		while ( !Frontier.ISEMPTY() ){
			leaf = new Node;
			*leaf = Frontier.POP();
			if ( problem.Goal_Test(*leaf) ){
				return this->BuildSolution(leaf);
			}
			// add the leaf node to explored set
			ExploredSet.PUSH(*leaf);
			// if the leaf is dead : no children delete it
			if ( problem.getTotalActionsForState(leaf->STATE()) == 0){
				delete leaf;
			} else {
			// add resulting nodes from expansion to frontier
			for ( int i = 1; i <= problem.getTotalActionsForState(leaf->STATE()); ++i){
				// skip back to parent
				Node child = problem.CHILDNODE(*leaf,problem.getActionIforState(leaf->STATE(),i));
				if ( !((leaf->PARENT() != 0) && (child.STATE() == leaf->PARENT()->STATE())) ){
					if ( !ExploredSet.DOEXIST(child.STATE()) ){
						Frontier.PUSH( child );
					}
				}
			}
			}
		}
		return this->BuildSolution(0);
	}
}
