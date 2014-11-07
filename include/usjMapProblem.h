/*
 * usjMapProblem.h
 *
 *  Created on: Jan 20, 2013
 *      Author: usama
 */

#ifndef USJMAPPROBLEM_H_
#define USJMAPPROBLEM_H_

#include "usjAILHead.h"

namespace usjAIL{

	class MapProblem : public Problem{
	public:
		MapProblem(const std::string filename);
		MapProblem(const State &istate,const State &goal,const std::string filename);
		virtual ~MapProblem();
		virtual bool Goal_Test(const Node &node) const;
		virtual State RESULT(State state,Action action) const;
		virtual double Heuristic(const State &forstate) const;
	protected:
		void initFromFile(const std::string &filename);
	public:
		double calculateDistance(const double &lat1,const double &long1,const double &lat2,
				const double &long2) const;
	};

}

#endif /* USJMAPPROBLEM_H_ */
