#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"
#include "../../Calibration.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class Up : public Step {
			public:
				Up(std::string name, Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys, Calibration& calibration) : 
					Step(name, seq, caller),
					controlSys(controlSys), 
					calibration(calibration){}
					
				int operator() () {return start();}
				int action(){
					eeros::math::Vector<4> torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit, q3gearTorqueLimit };
					controlSys.torqueLimitation.setLimit(-torqueLimit, torqueLimit);
					auto p = controlSys.pathPlanner.getLastPoint();
					
					p[2] = calibration.transportation_height;
					controlSys.pathPlanner.gotoPoint(p);
				};
				
				bool checkExitCondition(){
					controlSys.pathPlanner.posReached();
				}
			private:
		      
				DeltaControlSystem & controlSys;
				Calibration calibration;
		  };
	}
}