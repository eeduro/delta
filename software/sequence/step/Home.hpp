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
					
					controlSys.setVoltageForInitializing({2,2,2,7});
					
					wait(2);
					
					controlSys.enc1.callInputFeature<>("setFqdPos", q012homingOffset);
					controlSys.enc2.callInputFeature<>("setFqdPos", q012homingOffset);
					controlSys.enc3.callInputFeature<>("setFqdPos", q012homingOffset);

					controlSys.disableAxis();
					controlSys.voltageSetPoint.setValue({0.5,0.5,0.5,-7});
					wait(2);
					controlSys.voltageSetPoint.setValue({0.5,0.5,0.5,7});
					wait(0.2);
					controlSys.voltageSetPoint.setValue({0.5,0.5,0.5,0});
					controlSys.enc4.callInputFeature<>("resetFqd");
					wait(1);
					
					controlSys.enableAxis();
					
					controlSys.pathPlanner.gotoPoint({0,0,0,0});
					
					controlSys.disableAxis();
					
					while(true){
						wait(1);
						log.info() << controlSys.muxEnc.getOut().getSignal().getValue();
					}
					while(!controlSys.pathPlanner.posReached());
					
				};
		      
			private:
			
				DeltaControlSystem &controlSys;
				Wait wait;
				Calibration& calibration;
		};
	}
}