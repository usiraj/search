/*
 * usjMapProblem.cpp
 *
 *  Created on: Jan 20, 2013
 *      Author: usama
 */

#include "usjMapProblem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;
namespace usjAIL{
	MapProblem::MapProblem(const string filename){
		this->initFromFile(filename);
	}
	MapProblem::MapProblem(const State &istate,const State &goal,const std::string filename){
		this->initFromFile(filename);
		this->INITIAL_STATE = istate;
		this->FINAL_STATE = goal;
	}
	MapProblem::~MapProblem() { }
	bool MapProblem::Goal_Test(const Node &node) const{
	if ( Data::compareSTLStrings(node.STATE().getStateName(),this->FINAL_STATE.getStateName()) ){
			return true;
		}
		return false;
	}
	State MapProblem::RESULT(State state,Action action) const{
		string act;
		string name;
		int start,end;
		if (action.getTotalActions() < 1) return state;
		act = action.getAction(1);
		if ( this->ACTIONS.getActions(this->getStateWithName(state.getStateName())).actionExists(act) ){
			start = act.find("{")+1;
			end = act.find("}");
			return this->getStateWithName(act.substr(start,end-start));
		} else {
			return state;
		}
	}
	double MapProblem::Heuristic(const State &forstate) const{
		double lat1,lat2,long1,long2;
		State stg = this->getStateWithName(this->FINAL_STATE.getStateName());
		State stn = this->getStateWithName(forstate.getStateName());
		stn.get("Latitude",&lat1);	stn.get("Longitude",&long1);
		stg.get("Latitude",&lat2);	stg.get("Longitude",&long2);
		return this->calculateDistance(lat1,long1,lat2,long2);
	}
	void MapProblem::initFromFile(const string &filename){
		State *tStates = 0;
		Action *tActs = 0;
		ifstream file;
		string line,act;
		double dval;
		stringstream stream;
		bool newstate = false;
		file.open(filename.c_str());
		if ( !file ){
			cerr << "Error opening file" << endl;
			return;
		}
		file >> line;
		if ( !Data::compareSTLStrings(line,"Map")){
			// not a valid Map file
			cerr << "Not valid Map file" << endl;
			file.close();
			return;
		} else {
			// store map name
			line.clear();
			getline(file,line);
			stream.str(line);
			stream >> line;
			State ts = State("Map");
			ts.add("Map",line);
			this->ACTIONS.add(ts,Action());
		}
		while ( !file.eof() ){
			line.clear();
			getline(file,line);
			if ( line.empty()) break;
			if ( line.at(0) == '#' ) continue;	// ignore comment
			// got line
			if ( line.at(0) == '%' ) newstate = !newstate;
			if ( newstate ){
				if ( tStates == 0 ) tStates = new State();
				if ( tActs == 0 ) tActs = new Action();
					if ( line.at(0) == '$' ){
					// defining state
					stream.clear();
					stream.str(line.substr(1));
					stream >> line;
					stream >> dval;
					tStates->setStateName(line);
					tStates->add("Latitude",dval);
					stream >> dval;
					tStates->add("Longitude",dval);
				} else if ( line.at(0) == '@' ){
					// defining actions
					act.clear();
					stream.clear();
					stream.str(line.substr(1));
					stream >> act;
					while ( !stream.eof() ){
						stream >> line;
						if ( line.empty() ) break;
						stream >> dval;
						tActs->add(act + "{" + line + "}",dval);
					}
				}
			} else {
				if ( tStates != 0 && tActs != 0){
					this->ACTIONS.add(*tStates,*tActs);
					delete tStates;
					delete tActs;
				}
				tStates = 0;
				tActs = 0;
			}
		}
		file.close();
	}
	double MapProblem::calculateDistance(const double &lat1,const double &long1,const double &lat2,
			const double &long2) const{
		double d2r = M_PI / double(180);
		double R = 6367;	// mean earth radius in km
		double dlong = (long2 - long1) * d2r;
		double dlat = (lat2 - lat1) * d2r;
		double a = pow(sin(dlat/2.0), 2) + cos(lat1*d2r) * cos(lat2*d2r) * pow(sin(dlong/2.0), 2);
		double c = 2 * atan2(sqrt(a), sqrt(1-a));
		double d = R * c;
		return d;
	}
}
