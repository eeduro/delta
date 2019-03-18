#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"

#include "Wait.hpp"
#include "../../Calibration.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class Home : public Step{
			public:
				Home(std::string name, Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys,  Calibration& calibration) : 
					Step(name, seq, caller), controlSys(controlSys), 
					calibration(calibration),
					wait("wait", seq, this){
					
				}
				
				int action(){
					
					controlSys.setVoltageForInitializing({2,2,2,-7});
					
					wait(2.5);
					
					controlSys.enc1.callInputFeature<>("setFqdPos", q012homingOffset);
					controlSys.enc2.callInputFeature<>("setFqdPos", q012homingOffset);
					controlSys.enc3.callInputFeature<>("setFqdPos", q012homingOffset);
					
					wait(0.2);
					
					controlSys.disableAxis();
					
					controlSys.setVoltageForInitializing({0.5,0.5,0.5,-7});
					wait(0.5);
					controlSys.setVoltageForInitializing({0.5,0.5,0.5,0});
					controlSys.enc4.callInputFeature<>("resetFqd");
					wait(0.2);
					
					controlSys.pathPlanner.setInitPos({q012homingOffset,q012homingOffset,q012homingOffset,0});
					controlSys.enableAxis();
					wait(0.2);

					controlSys.pathPlanner.gotoPoint({0,0,calibration.transportation_height,0});
					
					
				};
				
				bool checkExitCondition(){
					controlSys.pathPlanner.posReached();
				}
		      
			private:
			
				DeltaControlSystem &controlSys;
				Wait wait;
				Calibration& calibration;
		};
	}
}