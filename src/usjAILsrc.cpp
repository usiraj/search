/*
 * usjAILsrc.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: usama
 */
#include "usjAILHead.h"
#include <iostream>
#include <sstream>

namespace usjAIL {
	// class State
	State::State(){
		std::string empty("");
		this->setStateName(empty);
	}
	State::State(const std::string &stateName){
		this->add("stateName",stateName);
	}
	State::~State(){}
	void State::setStateName(const std::string &stateName){
		if ( this->keyExists("stateName") ){
			this->modify("stateName",stateName);
		} else {
			this->add("stateName",stateName);
		}
	}
	std::string State::getStateName() const {
		std::string result;
		this->get("stateName",&result);
		return result;
	}
	void State::print() const{
		std::string key;
		std::string sval;
		bool bval;
		double dval;
		int ival;
		std::cout << "------------------------------------" << std::endl;
		for ( int i = 0; i < this->getSize() ; ++i ){
			key = this->getKey(i);
			std::cout << key << " : ";
			switch ( this->getType(key) ){
			case State::VALUE_BOOLEAN:
				this->get(key,&bval);
				std::cout << bval << std::endl;
				break;
			case State::VALUE_FLOAT:
				this->get(key,&dval);
				std::cout << dval << std::endl;
				break;
			case State::VALUE_INTEGER:
				this->get(key,&ival);
				std::cout << ival << std::endl ;
				break;
			case State::VALUE_STRING:
				this->get(key,&sval);
				std::cout << sval << std::endl;
				break;
			default:
				break;
			}
		}
		std::cout << "------------------------------------" << std::endl;
	}
	// class Action
	Action::Action(){
		this->nActions = 0;
		this->actions.unlock();
	}
	Action::Action(const Action &cpy){
		this->actions = cpy.actions;
		this->nActions = cpy.nActions;

	}
	Action& Action::operator=(const Action &rhs){
		this->actions = rhs.actions;
		this->nActions = rhs.getTotalActions();
		return *this;
	}
	Action::~Action(){
		this->nActions = 0;
	}
	void Action::add(const std::string &action,const double &stepcost){
		std::ostringstream stream,stream2;
		stream << "Action " << (this->nActions+1);
		std::string key = stream.str();
		stream2 << "StepCost " << (this->nActions+1);
		std::string key2 = stream2.str();
		if (!this->actionExists(action)){
			this->actions.add(key,action);
			this->actions.add(key2,stepcost);
			this->nActions = this->nActions+1;
		}
	}
	void Action::remove(const std::string &action){
		int index = -1;
		std::ostringstream str1,str2;
		for ( int i = 1 ; i <= this->nActions; ++i ){
			if ( Data::compareSTLStrings(action,this->getAction(i)) ){
				index = i;
			}
		}
		if ( index != -1 ){
			if ( index == this->nActions){
				str1 << "Action " << index;
				str2 << "StepCost " << index;
				this->actions.remove(str1.str());
				this->actions.remove(str2.str());
			} else {
				for ( int i = index; i < this->nActions; ++i){
					str1.str(std::string());	str2.str(std::string());
					str1 << "Action " << i;
					str2 << "StepCost " << i;
					this->actions.modify(str1.str(),this->getAction(i+1));
					this->actions.modify(str2.str(),this->getStepCost(i+1));
				}
				str1.str(std::string());	str2.str(std::string());
				str1 << "Action " << this->nActions;
				str2 << "StepCost " << this->nActions;
				this->actions.remove(str1.str());
				this->actions.remove(str2.str());
			}
			this->nActions = this->nActions -1 ;
		}
	}
	bool Action::actionExists(const std::string &action){
		std::string tempkey;
		Data::ValueType type;
		for ( int i = 0; i < this->actions.getSize();++i){
			tempkey = this->actions.getKey(i);
			type = this->actions.getType(tempkey);
			if ( type == Data::VALUE_STRING){
				if (this->actions.valueEqual(tempkey,action)){
					return true;
				}
			}
		}
		return false;
	}
	std::string Action::getAction(const int seed) const{
		std::string result("");
		std::ostringstream stream;
		std::string key;
		stream << "Action " << seed;
		key = stream.str();
		if ( seed > nActions) return result;
		this->actions.get(key,&result);
		return result;
	}
	double Action::getStepCost(const int seed) const {
		double result = -1;
		std::ostringstream stream;
		std::string key;
		stream << "StepCost " << seed;
		key = stream.str();
		if ( seed > nActions) return result;
		this->actions.get(key,&result);
		return result;
	}
	double Action::getStepCost(const std::string &action) const{
		int index = -1;
		std::ostringstream str1,str2;
		for ( int i = 1 ; i <= this->nActions; ++i ){
			if ( Data::compareSTLStrings(action,this->getAction(i)) ){
				index = i;
			}
		}
		if ( index != -1 ){
			return this->getStepCost(index);
		} else {
			return -1;
		}
	}
	int Action::getTotalActions() const{
		return this->nActions;
	}
	void Action::print() const{
		std::cout << "------------------------------------" << std::endl;
		std::cout << "Number of Actions : " << this->getTotalActions() << std::endl;
		for ( int i = 1; i <= this->getTotalActions(); ++i){
			std::cout << "Action " << i << " : "<< this->getAction(i) << "\t";
			std::cout << "StepCost " << i << " : " << this->getStepCost(i) << std::endl;
		}
		std::cout << "------------------------------------" << std::endl;
	}
	// class Actions
	Actions::Actions(){
		this->states = 0;
		this->actions = 0;
		this->nSize = 0;
	}
	Actions::Actions(const Actions &cpy){
		this->nSize = cpy.nSize;
		if ( this->nSize == 0 ){
			this->states = 0;
			this->actions = 0;
		} else {
			this->states = new State[this->nSize];
			this->actions = new Action[this->nSize];
			for ( unsigned int i = 0; i < this->nSize ; ++i ){
				this->states[i] = cpy.states[i];
				this->actions[i] = cpy.actions[i];
			}
		}
	}
	Actions& Actions::operator=(const Actions &rhs){
		this->nSize = rhs.getSize();
		if ( this->nSize == 0){
			this->states = 0;
			this->actions = 0;
		} else {
			this->states = new State[this->nSize];
			this->actions = new Action[this->nSize];
			for ( unsigned int i = 0; i < this->nSize; ++i){
				this->states[i] = rhs.getState(i);
				this->actions[i] =rhs.getActions(i);
			}
		}
		return *this;
	}
	Actions::~Actions(){
		this->clear();
	}
	void Actions::add(const State &state,const Action &action){
		State *tempStates;
		Action *tempActions;
		unsigned int newSize;
		if ( this->exist(state) ) this->modify(state,action);
		else {
			newSize = this->nSize+1;
			tempStates = new State[newSize];
			tempActions = new Action[newSize];
			for ( unsigned int i = 0; i < this->nSize; ++i){
				tempStates[i] = this->states[i];
				tempActions[i] = this->actions[i];
			}
			tempStates[newSize-1] = state;
			tempActions[newSize-1] = action;
			this->clear();
			this->nSize = newSize;
			this->states = tempStates;
			this->actions = tempActions;
		}
	}
	void Actions::remove(const State &state){
		int index = this->getIndex(state);
		if ( index != -1){
			this->remove(index);
		}
	}
	void Actions::remove(const unsigned int index){
		State *tempStates;
		Action *tempActions;
		unsigned int newSize;
		if  ( this->nSize == 1){
			this->clear();
			return;
		}
		if ( this->states != 0 && this->actions != 0){
			if ( index < this->nSize ){
				newSize = this->nSize-1;
				tempStates = new State[newSize];
				tempActions = new Action[newSize];
				for ( unsigned int i = 0, j = 0 ; i < this->nSize ; ++i ){
					if ( index != i){
						tempStates[j] = this->states[i];
						tempActions[j] = this->actions[i];
						++j;
					}
				}
				this->clear();
				this->nSize = newSize;
				this->states = tempStates;
				this->actions = tempActions;
			}
		}
	}
	void Actions::modify(const State &state,const Action &action){
		if ( this->exist(state) ){
			int index = this->getIndex(state);
			this->actions[index] = action;
		}
	}
	void Actions::clear(){
		if ( this->actions != 0 && this->states != 0){
			delete [] this->states;
			delete [] this->actions;
			this->states = 0;
			this->actions = 0;
			this->nSize = 0;
		}
	}
	bool Actions::exist(const State &state){
		if ( this->getIndex(state) == -1) return false;
		return true;
	}
	Action Actions::getActions(const State &state) const{
		Action failAction;
		failAction.add("FAILED");
		int index = this->getIndex(state);
		if ( index == -1) return failAction;
		return this->getActions(index);
	}
	Action Actions::getActions(const unsigned int index) const{
		Action failAction;
		failAction.add("FAILED");
		if ( this->actions == 0) return failAction;
		if ( index >= this->nSize ) return failAction;
		return this->actions[index];
	}
	State Actions::getState(unsigned int index) const{
		State failState;
		failState.setStateName("FAILED");
		if ( this->states == 0) return failState;
		if ( index >= this->nSize ) return failState;
		return this->states[index];
	}
	int Actions::getIndex(const State &state) const{
		if ( this->states == 0) return -1;
		for ( unsigned int i = 0; i < this->nSize ; ++i){
			if ( this->states[i] == state){
				return i;
			}
		}
		return -1;
	}
	unsigned int Actions::getSize() const{
		return this->nSize;
	}
	void Actions::print() const{
		std::cout << "#######################" << std::endl;
		for (unsigned int i = 0; i < this->nSize; ++i ){
			this->states[i].print();
			this->actions[i].print();
		}
		std::cout << "#######################" << std::endl;
	}
	// class Node
	Node::Node(){
		this->parent = 0;
		this->pathCost = 0;
		this->futureCost = 0;
	}
	Node::Node(const State &iState){
		this->state = iState;
		this->parent = 0;
		this->pathCost = 0;
		this->futureCost = 0;
	}
	Node::Node(const std::string &stateName)
	:state(stateName){
		this->parent = 0;
		this->pathCost = 0;
		this->futureCost = 0;
	}
	Node::Node(const Node &cpy){
		this->parent = cpy.parent;
		this->action = cpy.action;
		this->state = cpy.state;
		this->pathCost = cpy.pathCost;
		this->futureCost = cpy.futureCost;
	}
	Node& Node::operator=(const Node &rhs){
		this->parent = rhs.parent;
		this->futureCost = rhs.futureCost;
		this->pathCost = rhs.pathCost;
		this->state = rhs.state;
		this->action = rhs.action;
		return *this;
	}
	Node::~Node(){
		this->parent = 0;
	}
	State Node::STATE() const{
		return this->state;
	}
	Node* Node::PARENT() const{
		return this->parent;
	}
	double Node::PATH_COST() const{
		return this->pathCost;
	}
	double Node::FUTURE_COST() const{
		return this->futureCost;
	}
	Action Node::getAction() const{
		return this->action;
	}
	void Node::setParent(const Node* parentNode){
		this->parent = const_cast<Node*>(parentNode);
	}
	void Node::setState(const State &statetoset){
		this->state = statetoset;
	}
	void Node::setAction(const Action &actiontoset){
		this->action = actiontoset;
	}
	void Node::setPathCost(const double pathCosttoSet){
		this->pathCost = pathCosttoSet;
	}
	void Node::setFutureCost(const double futureCosttoSet){
		this->futureCost = futureCosttoSet;
	}
	void Node::print() const{
		std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
		if ( this->parent == 0) std::cout << "Root Node" << std::endl;
		else{
			std::cout << "Parent :" << std::endl;
			this->parent->state.print();
		}
		this->state.print();
		this->action.print();
		std::cout << "Path Cost : " << this->pathCost << std::endl;
		std::cout << "Future Cost : " << this->futureCost << std::endl;
		std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
	}
	// class Stack
	Stack::Stack(){
		this->type = Stack::FIFO;
		this->nSize = 0;
		this->nodes = 0;
	}
	Stack::Stack(const Stack &cpy){
		this->nSize = cpy.nSize;
		this->type = cpy.type;
		if ( this->nSize != 0){
			this->nodes = new Node[this->nSize];
			for ( unsigned int i = 0; i < this->nSize ; ++i){
				this->nodes[i] = cpy.nodes[i];
			}
		} else {
			this->nodes = 0;
		}
	}
	Stack& Stack::operator=(const Stack &rhs){
		this->nSize = rhs.nSize;
		this->type = rhs.type;
		if ( this->nSize != 0){
			this->nodes = new Node[this->nSize];
			for ( unsigned int i = 0; i < this->nSize ; ++i){
				this->nodes[i] = rhs.nodes[i];
			}
		} else {
			this->nodes = 0;
		}
		return *this;
	}
	Stack::~Stack(){
		this->clear();
	}
	void Stack::setStackType(StackType typeofStack){
		this->type = typeofStack;
	}
	Stack::StackType Stack::getStackType() const{
		return this->type;
	}
	unsigned int Stack::getSize() const{
		return this->nSize;
	}
	Node Stack::POP(){
		Node result("FAILED");
		int index = 0;
		if ( this->nSize == 0) return result;
		switch ( this->type){
		case Stack::FIFO:
			index = 0;
			break;
		case Stack::LIFO:
			index = this->nSize -1;
			break;
		case Stack::PRIORITY_PATH:
			index = this->minPathCostIndex();
			break;
		case Stack::PRIORITY_EXPECTED:
			index = this->minFutureCostIndex();
			break;
		case Stack::PRIORITY_COMBINED:
			index = this->minCombinedCostIndex();
			break;
		default:	// behave as fifo
			index = 0;
			break;
		}
		if ( index != -1){
			result = this->nodes[index];
			this->remove(index);
		}
		return result;
	}
	void Stack::PUSH(const Node &node){
		Node* tempStack;
		unsigned int newSize = this->nSize+1;
		tempStack = new Node[newSize];
		// copy old nodes
		for ( unsigned int i = 0; i < this->nSize ; ++i){
			tempStack[i] = this->nodes[i];
		}
		tempStack[newSize-1] = node;
		this->clear();
		this->nSize = newSize;
		this->nodes = tempStack;
	}
	void Stack::remove(const State &stateNode){
		int index = this->getIndex(stateNode);
		if ( index != -1){
			this->remove(index);
		}
	}
	void Stack::removeDuplicates(const State &stateNode){
		if ( this->nSize < 2) return;
		double cost1,cost2;
		int size = this->nSize;
		int minIndex = 0;
		int* dupIndices = new int[size];
		for ( int i = 0; i < size ; ++i) dupIndices[i] = -1;
		for ( unsigned int i = 0, j = 0; i < this->nSize; ++i ){
			if ( this->nodes[i].STATE() == stateNode ){
				dupIndices[j] = i;
				++j;
			}
		}
		if ( dupIndices[0] == -1 ) return;
		minIndex = dupIndices[0];
		for ( int i = 0; (dupIndices[i] != -1) && ( i < size); ++i){
			switch ( this->type ){
			case Stack::FIFO:
				minIndex = dupIndices[0];
				break;
			case Stack::LIFO:
				minIndex = dupIndices[i];
				break;
			case Stack::PRIORITY_PATH:
				if ( this->nodes[dupIndices[i]].PATH_COST() < this->nodes[minIndex].PATH_COST() ){
					minIndex = dupIndices[i];
				}
				break;
			case Stack::PRIORITY_EXPECTED:
				if ( this->nodes[dupIndices[i]].FUTURE_COST() < this->nodes[minIndex].FUTURE_COST() ){
					minIndex = dupIndices[i];
				}
				break;
			case Stack::PRIORITY_COMBINED:
				cost1 = this->nodes[dupIndices[i]].PATH_COST() + this->nodes[dupIndices[i]].FUTURE_COST();
				cost2 = this->nodes[minIndex].PATH_COST() + this->nodes[minIndex].FUTURE_COST();
				if ( cost1 < cost2 ){
					minIndex = dupIndices[i];
				}
				break;
			default:
				minIndex = dupIndices[0];
				break;
			}
		}
		for ( int i = 0; (dupIndices[i] != -1) && ( i < size ) ; ++i ){
			if ( dupIndices[i] != minIndex ){
				this->remove(dupIndices[i]);
			}
		}
		delete [] dupIndices;
	}
	bool Stack::ISEMPTY() const{
		if ( this->nSize == 0) return true;
		return false;
	}
	bool Stack::DOEXIST(const State &stateNode) const{
		if ( this->getIndex(stateNode) == -1) return false;
		return true;
	}
	void Stack::clear(){
		if (this->nodes != 0) delete [] nodes;
		this->nodes = 0;
		this->nSize = 0;
	}
	// utility functions of Stack
	void Stack::remove(const unsigned int index){
		if ( this->nSize == 0) return;
		if ( index >= this->nSize ) return;
		if ( this->nSize == 1){
			this->clear();
			return;
		}
		Node *tempStack;
		unsigned int newSize = this->nSize - 1;
		tempStack = new Node[newSize];
		for ( unsigned int i = 0, j = 0; i < this->nSize; ++i){
			if ( index != i){
				tempStack[j] = this->nodes[i];
				++j;
			}
		}
		this->clear();
		this->nSize = newSize;
		this->nodes = tempStack;
	}
	int Stack::getIndex(const State &stateNode) const{
		if ( this->nSize == 0) return -1;
		for ( unsigned int i = 0; i < this->nSize ; ++i){
			if ( this->nodes[i].STATE() == stateNode ) return i;
		}
		return -1;
	}
	int Stack::minPathCostIndex() const{
		if ( this->nSize == 0) return -1;
		int minIndex = 0;
		for ( unsigned int i = 0; i < this->nSize; ++i){
			if ( this->nodes[i].PATH_COST() < this->nodes[minIndex].PATH_COST() ){
				minIndex = i;
			}
		}
		return minIndex;
	}
	int Stack::minFutureCostIndex() const{
		if ( this->nSize == 0) return -1;
		int minIndex = 0;
		for ( unsigned int i = 0; i < this->nSize; ++i){
			if ( this->nodes[i].FUTURE_COST() < this->nodes[minIndex].FUTURE_COST() ){
				minIndex = i;
			}
		}
		return minIndex;
	}
	int Stack::minCombinedCostIndex() const{
		if ( this->nSize == 0) return -1;
		int minIndex = 0;
		double cost1;
		double cost2;
		for ( unsigned int i = 0; i < this->nSize; ++i){
			cost1 = this->nodes[i].PATH_COST() + this->nodes[i].FUTURE_COST();
			cost2 = this->nodes[minIndex].PATH_COST() + this->nodes[minIndex].FUTURE_COST();
			if ( cost1 < cost2 ){
				minIndex = i;
			}
		}
		return minIndex;
	}
	// class Problem
	Problem::Problem(){ }
	Problem::Problem(const State &istate,const State &goal,const Actions actions){
		this->INITIAL_STATE = istate;
		this->FINAL_STATE = goal;
		this->ACTIONS = actions;
	}
	Problem::Problem(const Problem &cpy){
		this->INITIAL_STATE = cpy.INITIAL_STATE;
		this->FINAL_STATE = cpy.FINAL_STATE;
		this->ACTIONS = cpy.ACTIONS;
	}
	Problem& Problem::operator=(const Problem &rhs){
		this->INITIAL_STATE = rhs.INITIAL_STATE;
		this->FINAL_STATE = rhs.FINAL_STATE;
		this->ACTIONS = rhs.ACTIONS;
		return *this;
	}
	Problem::~Problem(){}
	bool Problem::Goal_Test(const Node &node) const{
		if ( node.STATE() == this->FINAL_STATE ) return true;
		return false;
	}
	double Problem::Step_Cost(const State &instate,const Action &action) const{
		Action acts = this->ACTIONS.getActions(instate);
		if ( Data::compareSTLStrings(acts.getAction(1),"FAILED") ) return -1;
		if ( action.getTotalActions() < 1 ) return -1;
		if ( !acts.actionExists(action.getAction(1)) ) return -1;
		return acts.getStepCost(action.getAction(1));
	}
	void Problem::defineActions(const Actions &actions){
		this->ACTIONS = actions;
	}
	void Problem::defineInitialState(const State &istate){
		this->INITIAL_STATE = istate;
	}
	void Problem::defineGoalState(const State &goalstate){
		this->FINAL_STATE = goalstate;
	}
	State Problem::getInitialState() const{
		return this->INITIAL_STATE;
	}
	State Problem::getGoalState() const{
		return this->FINAL_STATE;
	}
	Actions Problem::getActions() const{
		return this->ACTIONS;
	}
	Action Problem::getActionIforState(const State &state,const int index) const{
		Action result;
		result.add(this->ACTIONS.getActions(state).getAction(index),
				this->ACTIONS.getActions(state).getStepCost(index));
		return result;
	}
	int Problem::getTotalActionsForState(const State &state) const {
		return this->ACTIONS.getActions(state).getTotalActions();
	}
	Node Problem::CHILDNODE(const Node &node,const Action &action) const{
		Node childNode;
		childNode.setParent(&node);	// setting parent
		childNode.setState(this->RESULT(node.STATE(),action));		// setting state
		childNode.setAction(action);	// setting action
		childNode.setPathCost(node.PATH_COST() +
				this->Step_Cost(node.STATE(),action) );// setting path cost
		childNode.setFutureCost(this->Heuristic(childNode.STATE()));	// setting future cost
		return childNode;
	}
	State Problem::getStateWithName(const std::string &stateName) const{
		for (unsigned int i = 0; i < this->ACTIONS.getSize(); ++i){
			if ( Data::compareSTLStrings(this->ACTIONS.getState(i).getStateName(),stateName ) ){
				return this->ACTIONS.getState(i);
			}
		}
		return State("FAILED");
	}
	void printResult(Stack &result){
		Node temp;
		result.setStackType(Stack::LIFO);
		if ( result.ISEMPTY() ) {
			std::cout << "No solution found" << std::endl;
		} else {
			temp = result.POP();
			if ( Data::compareSTLStrings(temp.STATE().getStateName(),"FAILED") ){
				std::cout << " Solution Failed" << std::endl;
			} else {
				std::cout << " Solution Found - Path is " << std::endl;
				std::cout << "1- ";
				temp.STATE().print();
				std::cout << "Path Cost till now: " << temp.PATH_COST() << std::endl;
				for ( int i = 2; !result.ISEMPTY(); ++i){
					temp = result.POP();
					std::cout << i << "- ";
					temp.STATE().print();
					std::cout << "Reach here by " <<temp.getAction().getAction(1) << std::endl;
					std::cout << "Path Cost till now: " << temp.PATH_COST() << std::endl;
					std::cout << "Expected Cost :" << temp.FUTURE_COST() << std::endl;
				}
			}
		}
	}
}
