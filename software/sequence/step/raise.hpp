#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"

#include "delay.hpp"

namespace eeduro{
	namespace delta{
		class Raise : public eeros::sequencer::Step{
			public:
				Raise(std::string name,eeros::sequencer::Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys) : 
					Step(name, seq, caller), 
					controlSys(controlSys),
					delay("delay", seq, this){
					
				}
				
				int operator() () {return Step::start();}
				
				int action(){	
					controlSys.start();
					eeros::math::Vector<4> torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit, q3gearTorqueLimit };
					controlSys.torqueLimitation.setLimit(-torqueLimit, torqueLimit);
					controlSys.setVoltageForInitializing({2,2,2,7});
					
					delay(2);
					/*usleep(2000000);*/
										
					controlSys.enc1.callInputFeature<>("resetFqd");
					controlSys.enc2.callInputFeature<>("resetFqd");
					controlSys.enc3.callInputFeature<>("resetFqd");
					controlSys.enc4.callInputFeature<>("resetFqd");
				};
		      
			private:			
				DeltaControlSystem &controlSys;
				Delay delay;
		};
	}
}