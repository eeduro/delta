#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"
#include "../../Calibration.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class Move : public Step {
			public:
			Move(std::string name, Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys) : 
				Step(name, seq, caller), 
				controlSys(controlSys){
				this->position = {0,0,0,0};
			}
			int operator() (AxisVector position) {this->position = position; return start();}
			int action(){
				controlSys.pathPlanner.gotoPoint(position);
			};
			bool checkExitCondition(){
					return controlSys.pathPlanner.posReached();
			}
			
		private:
			DeltaControlSystem &controlSys;
			AxisVector position;
		      
		  };
	}
}