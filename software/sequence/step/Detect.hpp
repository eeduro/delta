#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"
#include "../../Calibration.hpp"

#include "Wait.hpp"

#include <unistd.h>

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class Detect : public Step {
			public:
				Detect(std::string name, Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys, Calibration& calibration) : 
					Step(name, seq, caller),
					controlSys(controlSys),
					calibration(calibration),
					wait("wait", seq, this){
				}
		      
				int getBlock(){
					return blockNumber;
				}
		      
				int operator() (int pos) {this->position = pos; return start();}
			
				int action(){
					double down = calibration.position[position].level12 + 0.002;
					double touch = calibration.position[position].level30 - 0.0002;
				
					eeros::math::Vector<4> torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit, q3gearTorqueLimit };
					eeros::math::Vector<4> torqueLimitDown = torqueLimit * 0.1;
					eeros::math::Vector<4> zero = torqueLimit * 0.01;
				
					auto p = controlSys.pathPlanner.getLastPoint();
					double last_z = p[2];
					p[2] = down;
					controlSys.pathPlanner.gotoPoint(p);
					waitUntilPointReached();
				
					controlSys.torqueLimitation.setLimit(-torqueLimitDown, torqueLimitDown);
				
					p[2] = touch;
					controlSys.pathPlanner.gotoPoint(p);
					waitUntilPointReached();
				
					controlSys.torqueLimitation.setLimit(-zero, zero);
					usleep(500000);
					double z = controlSys.directKin.getOut().getSignal().getValue()[2];
					controlSys.torqueLimitation.setLimit(-torqueLimitDown, torqueLimitDown);
					
					p[2] = down;
					controlSys.pathPlanner.gotoPoint(p);
					waitUntilPointReached();
					
					controlSys.torqueLimitation.setLimit(-torqueLimit, torqueLimit);
					
					p[2] = last_z;
					controlSys.pathPlanner.gotoPoint(p);
					waitUntilPointReached();
					
					int block = calibration.getBlock(position, z);
					log.trace() << "[DETECT] pos " << position << ": z = " << z << " -> block = " << block;

					blockNumber = block;
					
					return block;
				};
			
			private:
				void waitUntilPointReached(){
					while (!controlSys.pathPlanner.posReached()) {
						wait(0.1);
					}
				};
		      
				DeltaControlSystem &controlSys;
				Calibration calibration;
				int position;
				int blockNumber;
				Wait wait;
		      
		};

	}
}