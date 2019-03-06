#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"
#include "../../Calibration.hpp"

#include "Wait.hpp"

namespace eeduro{
	namespace delta{
		class Up : public eeros::sequencer::Step {
			public:
				Up(std::string name,eeros::sequencer::Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys, Calibration& calibration) : 
					Step(name, seq, caller),
					controlSys(controlSys), 
					calibration(calibration),
					wait("wait", seq, this){}
					
				int operator() () {return Step::start();}
				int action(){
					eeros::math::Vector<4> torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit, q3gearTorqueLimit };
					controlSys.torqueLimitation.setLimit(-torqueLimit, torqueLimit);
					auto p = controlSys.pathPlanner.getLastPoint();
					
					p[2] = calibration.transportation_height;
					controlSys.pathPlanner.gotoPoint(p);
					waitUntilPointReached();
				};
			private:
				void waitUntilPointReached(){
					while (!controlSys.pathPlanner.posReached()) {
						wait(0.1);
					} 
				};
		      
			DeltaControlSystem & controlSys;
			Calibration calibration;
			Wait wait;
		  };
	}
}