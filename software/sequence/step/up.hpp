#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"
#include "../../Calibration.hpp"

#include "delay.hpp"

namespace eeduro{
	namespace delta{
		class Up : public eeros::sequencer::Step {
			public:
				Up(std::string name,eeros::sequencer::Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys, Calibration& calibration) : 
					Step(name, seq, caller),
					controlSys(controlSys), 
					calibration(calibration),
					delay("delay", seq, this){}
					
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
						/*usleep(100000);
						std::this_thread::yield();*/
					} 
				};
		      
			DeltaControlSystem & controlSys;
			Calibration calibration;
			Delay delay;
		  };
	}
}