#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>

#include "../../control/DeltaControlSystem.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class Move : public Step {
			public:
				Move(std::string name, Sequence* caller, DeltaControlSystem& controlSys) : Step(name, caller), controlSys(controlSys) {
					this->position = {0,0,0};
				}
				int operator() (AxisVector position) {this->position = position; return start();}
				int action() {controlSys.pathPlanner.gotoPoint(position);}
				bool checkExitCondition() {return controlSys.pathPlanner.posReached();}
				
			private:
				DeltaControlSystem &controlSys;
				AxisVector position;
		};
	}
}