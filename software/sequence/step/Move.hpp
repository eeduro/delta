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
			Move(std::string name, Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys, Calibration& calibration) : 
				Step(name, seq, caller), 
				controlSys(controlSys), 
				calibration(calibration){
				this->position = 0;
			}
			int operator() (int pos) {this->position = pos; return start();}
			int action(){
				auto p = controlSys.pathPlanner.getLastPoint();
				p[0] = calibration.position[position].x;
				p[1] = calibration.position[position].y;			
				
				if (p[3] > 1) {
					p[3] = calibration.position[position].r;
				}
				else {
					p[3] = calibration.position[position].r + pi / 2.0;
					
				}
				controlSys.pathPlanner.gotoPoint(p);
			};
			bool checkExitCondition(){
					controlSys.pathPlanner.posReached();
			}
			
		private:
		      
			DeltaControlSystem &controlSys;
			Calibration calibration;
			int position;
		      
		  };
	}
}