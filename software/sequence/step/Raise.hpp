#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"

#include "Wait.hpp"

namespace eeduro{
	namespace delta{
		class Raise : public eeros::sequencer::Step{
			public:
				Raise(std::string name,eeros::sequencer::Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys) : 
					Step(name, seq, caller), 
					controlSys(controlSys),
					wait("wait", seq, this){
					
				}
				
				int action(){	
					controlSys.setVoltageForInitializing({2,2,2,7});
					
					wait(2);
				
					controlSys.enc1.callInputFeature<>("resetFqd");
					controlSys.enc2.callInputFeature<>("resetFqd");
					controlSys.enc3.callInputFeature<>("resetFqd");
					controlSys.enc4.callInputFeature<>("resetFqd");
				};
		      
			private:			
				DeltaControlSystem &controlSys;
				Wait wait;
		};
	}
}