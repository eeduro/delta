#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"
#include "../../Calibration.hpp"

#include <unistd.h>

namespace eeduro{
  namespace delta{
		  class Down : public eeros::sequencer::Step {
		    public:
		      Down(std::string name,eeros::sequencer::Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys, Calibration& calibration) : Step(name, seq, caller), controlSys(controlSys), calibration(calibration){
			this->position = 0;
		      }
		      int operator() (int pos) {this->position = pos; return Step::start();}
		      int action(){			
			double down = calibration.position[position].zblock[1] + 0.001;
			double touch = calibration.position[position].zblock[3];
			
			eeros::math::Vector<4> torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit, q3gearTorqueLimit };
			eeros::math::Vector<4> torqueLimitDown = torqueLimit * 0.1;
			
			
			auto p = controlSys.pathPlanner.getLastPoint();
			
			p[2] = down;
			controlSys.pathPlanner.gotoPoint(p);
			waitUntilPointReached();
			
			controlSys.torqueLimitation.setLimit(-torqueLimitDown, torqueLimitDown);
			
			p[2] = touch;
			controlSys.pathPlanner.gotoPoint(p);
			waitUntilPointReached();
		      };
		      
		    private:
		      void waitUntilPointReached(){
			while (!controlSys.pathPlanner.posReached()) {
				usleep(100000);
				std::this_thread::yield();
			} 
		      };
		      
		      DeltaControlSystem &controlSys;
		      Calibration calibration;
		      int position;
		    
		  };

	}
}