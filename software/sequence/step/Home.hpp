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
				Home(std::string name, Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys) : 
					Step(name, seq, caller), controlSys(controlSys), 
					wait("wait", seq, this){
					
				}
				
				int action(){
					controlSys.setVoltageForInitializing({2,2,2,-7});
					
					wait(2.5);
					
					controlSys.enc1.callInputFeature<>("setFqdPos", q012homingOffset);
					controlSys.enc2.callInputFeature<>("setFqdPos", q012homingOffset);
					controlSys.enc3.callInputFeature<>("setFqdPos", q012homingOffset);
					controlSys.enc4.callInputFeature<>("resetFqd");
					
					wait(0.1);
					
					controlSys.pathPlanner.setInitPos(controlSys.directKin.getOut().getSignal().getValue());
					
					wait(0.1);
					
					controlSys.enableAxis();
				};      
			private:
				DeltaControlSystem &controlSys;
				Wait wait;
		};
	}
}