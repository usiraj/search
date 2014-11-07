/*
 * usjAILHead.h
 *
 *  Created on: Jan 17, 2013
 *      Author: usama
 */

#ifndef USJAILHEAD_H_
#define USJAILHEAD_H_

#include <string>
#include <vector>
#include <map>

namespace usjAIL{

	class Data{
	public:
		enum ValueType { UNSUCCESSFUL = -1,VALUE_BOOLEAN = 1,	VALUE_INTEGER = 2,
			VALUE_FLOAT = 4, VALUE_STRING = 8};
		// constructors
		Data();
		Data(const Data &cpy);
		Data &operator=(const Data &rhs);
		virtual ~Data();
		// functions
		void add(const std::string &key,const double value);
		void add(const std::string &key,const int value);
		void add(const std::string &key,const bool value);
		void add(const std::string &key,const std::string &value);
		// getting stored states
		ValueType getType(const std::string &key) const;
		std::string getKey(const int index) const;
		void get(const std::string &key, double *valuetoget) const;
		void get(const std::string &key, int *valuetoget) const;
		void get(const std::string &key, bool *valuetoget) const;
		void get(const std::string &key, std::string *valuetoget) const;
		// modifying
		void modify(const std::string &key,const double newvalue);
		void modify(const std::string &key,const int newvalue);
		void modify(const std::string &key,const bool newvalue);
		void modify(const std::string &key,const std::string &newvalue);
		// removing
		void remove(const std::string &key);
		void clear(ValueType type);
		// searching and etc
		bool keyExists(const std::string &key) const;
		bool valueEqual(const std::string &key, const double cmpValue) const;
		bool valueEqual(const std::string &key, const int cmpValue) const;
		bool valueEqual(const std::string &key, const bool cmpValue) const;
		bool valueEqual(const std::string &key, const std::string &cmpValue) const;
		void lock() const;
		void unlock();
		bool islock() const;
		// size functions
		int getSize() const;
		int getSize(ValueType oftype) const;
		// Comparison of state spaces
		bool operator==(const Data &cmpstate) const;
		bool operator!=(const Data &cmpstate) const;
		bool sameType(const Data &cmpstate) const;
		// debug print
		virtual void print() const;
	protected:
		std::vector<std::string> *kMap;
		std::map<std::string,double> *dMap;
		std::map<std::string,int> *iMap;
		std::map<std::string,bool> *bMap;
		std::map<std::string,std::string> *sMap;
		// functions
		bool keyExists(const std::string &key, ValueType oftype) const;
	private:
		mutable bool locked;
	public:
		static bool compareSTLStrings(const std::string &str1,const std::string &str2);
	};
	class State : public Data{
	public:
		State();
		State(const std::string &stateName);
		virtual ~State();
		void setStateName(const std::string &stateName);
		std::string getStateName() const;
		virtual void print() const;
	};
	class Problem;
	class Action{
		/*	Actions are added in the form ACTION{state_name}
		 *  default step cost is one
		 *  seed starts from one
		 */
	public:
		Action();
		Action(const Action &cpy);
		Action &operator=(const Action &rhs);
		virtual ~Action();
		void add(const std::string &action,const double &stepCost = 1);
		void remove(const std::string &action);
		bool actionExists(const std::string &action);
		//
		std::string getAction(const int seed) const;
		double getStepCost(const int seed) const;
		double getStepCost(const std::string &action) const;
		int getTotalActions() const;
		// debug print
		virtual void print() const;
	protected:
		Data actions;
		int nActions;
	};
	class Actions{
		// class for holding legal actions for every state
	public:
		Actions();
		Actions(const Actions &cpy);
		Actions &operator=(const Actions &rhs);
		virtual ~Actions();
		void add(const State &state,const Action &action);
		void remove(const State &state);
		void remove(const unsigned int index);
		void modify(const State &state,const Action &action);
		void clear();
		bool exist(const State &state);
		Action getActions(const State &state) const;
		Action getActions(const unsigned int index) const;
		State getState(const unsigned int index) const;
		int getIndex(const State &state) const;
		unsigned int getSize() const;
		virtual void print() const;
	protected:
		State *states;
		Action *actions;
		unsigned int nSize;
	private:
	};
	class Node{
	public:
		// constructors / destructors
		Node();
		Node(const State &initialstate);	// for root nodes
		Node(const std::string &stateName);	// for atomic root nodes
		Node(const Node &cpy);
		Node &operator=(const Node &rhs);
		virtual ~Node();
		// member functions
		State STATE() const;
		Node* PARENT() const;
		double PATH_COST() const;
		double FUTURE_COST() const;
		Action getAction() const;
		void setParent(const Node* parentNode);
		void setState(const State &statetoset);
		void setAction(const Action &actiontoset);
		void setPathCost(const double pathCost);
		void setFutureCost(const double futureCost);
		virtual void print() const;
	protected:
		State state;
		Action action;
		Node* parent;
		double pathCost;
		double futureCost;
	};
	class Stack{
		/*
		 * A generic Stack implementing FIFO , LIFO and Priority Queue
		 */
	public:
		enum StackType { FIFO = 1, LIFO = 2, PRIORITY_PATH ,
			PRIORITY_EXPECTED ,PRIORITY_COMBINED};
		// constructors
		Stack();
		Stack(const Stack &cpy);
		Stack &operator=(const Stack &rhs);
		virtual ~Stack();
		// member functions
		void setStackType(StackType typeofStack);
		StackType getStackType() const;
		unsigned int getSize() const;
		Node POP();
		void PUSH(const Node &node);
		void remove(const State &stateNode);
		void removeDuplicates(const State &stateNode);
		bool ISEMPTY() const;
		bool DOEXIST(const State &stateNode) const;
		void clear();
	protected:
		// data members
		Node* nodes;
		unsigned int nSize;
		StackType type;
	private:
		// utility functions
		void remove(const unsigned int index);
		int getIndex(const State &stateNode) const;
		int minPathCostIndex() const;
		int minFutureCostIndex() const;
		int minCombinedCostIndex() const;
	};

	class Problem{
		/*
		 * Abstract Problem class
		 * To inherit it, Heuristic and RESULT must be defined
		 */
	public:
		Problem();
		Problem(const State &istate,const State &goal,const Actions actions);
		Problem(const Problem &cpy);
		Problem &operator=(const Problem &rhs);
		virtual ~Problem();
	protected:
		// data members
		State INITIAL_STATE;
		State FINAL_STATE;
		Actions ACTIONS;
		// member functions
	public:
		double Step_Cost(const State &instate,const Action &action) const;
		void defineActions(const Actions &actions);
		void defineInitialState(const State &istate);
		void defineGoalState(const State &goalstate);
		State getInitialState() const;
		State getGoalState() const;
		State getStateWithName(const std::string &stateName) const;
		Actions getActions() const;
		Action getActionIforState(const State &state,const int index) const;	// index starting from one
		int getTotalActionsForState(const State &state) const;
		Node CHILDNODE(const Node &node,const Action &action) const;
		virtual bool Goal_Test(const Node &node) const;
		virtual double Heuristic(const State &forstate) const = 0;
		virtual State RESULT(State state,Action action) const = 0;
	};
	void printResult(Stack &result);
}

#endif /* USJAILHEAD_H_ */
