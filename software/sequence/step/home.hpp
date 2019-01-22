#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"

#include "delay.hpp"

namespace eeduro{
	namespace delta{
		class Home : public eeros::sequencer::Step{
			public:
				Home(std::string name,eeros::sequencer::Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys) : 
					Step(name, seq, caller), controlSys(controlSys), 
					mot1V(0.0), mot2V(0.0), mot3V(0.0), mot4V(0.0),
					mot1Homed(false), mot2Homed(false), mot3Homed(false), mot4Homed(false),
					mot1(0.0), mot2(0.0), mot3(0.0), mot4(0.0),
					delay("delay", seq, this){
					
				}
				
				int operator() () {return Step::start();}
				
				int action(){							
					while(!mot1Homed || !mot2Homed || !mot3Homed){
 						mot1 = controlSys.enc1.getOut().getSignal().getValue();
						mot2 = controlSys.enc2.getOut().getSignal().getValue();
						mot3 = controlSys.enc3.getOut().getSignal().getValue();
						
						if(mot1 < -0.453785606 && mot1 > -0.46){	//453785606
							mot1V = 0;
							mot1Homed = true;
						}else{
							if(mot1 >= -0.453785606) mot1V = -2; //-2
							else mot1V = 2; //2
							mot1Homed = false;
						}
						
						if(mot2 < -0.453785606 && mot2 > -0.46){
							mot2V = 0;
							mot2Homed = true;
						}else{
							if(mot2 >= -0.453785606) mot2V = -2; //-2
							else mot2V = 2;//2
							mot2Homed = false;
						}
						
						if(mot3 < -0.453785606 && mot3 > -0.46){
							mot3V = 0;
							mot3Homed = true;
						}else{
							if(mot3 >= -0.453785606) mot3V = -2;//-2
							else mot3V = 2;//2
							mot3Homed = false;
						}
											
						if(mot1Homed && mot2Homed && mot3Homed){
							controlSys.setVoltageForInitializing({0,0,0,0});
							controlSys.enc1.callInputFeature<>("resetFqd");
							controlSys.enc2.callInputFeature<>("resetFqd");
							controlSys.enc3.callInputFeature<>("resetFqd");
							controlSys.enc4.callInputFeature<>("resetFqd");
							controlSys.mouse.reset(0,0,0,0);
							controlSys.voltageSwitch.switchToInput(0);
							controlSys.homed = true;
						}else{
							controlSys.voltageSetPoint.setValue({mot1V, mot2V, mot3V, -7.0});
						}
					}
					
					controlSys.disableAxis();

					controlSys.voltageSetPoint.setValue({0.5,0.5,0.5,-7});
					
					delay(2);
				
					
					controlSys.voltageSetPoint.setValue({0.5,0.5,0.5,7});
					
					delay(0.2);
					
					controlSys.enc4.callInputFeature<>("resetFqd");
				};
		      
			private:
			
				DeltaControlSystem &controlSys;
				double mot1V;
				double mot2V;
				double mot3V;
				double mot4V;
				bool mot1Homed;
				bool mot2Homed;
				bool mot3Homed;
				bool mot4Homed;
				double mot1;
				double mot2;
				double mot3;
				double mot4;
				Delay delay;
		};
	}
}