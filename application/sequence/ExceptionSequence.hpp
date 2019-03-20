#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "../Calibration.hpp"

#include <vector>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class MouseExceptionSequence : public Sequence{
			public:
				MouseExceptionSequence(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& safetySys, DeltaSafetyProperties& properties, DeltaControlSystem& controlSys):
				Sequence(name, sequencer, caller, true),
				controlSys(controlSys),
				safetySys(safetySys),
				properties(properties){}
				
				int action(){
					controlSys.setMouseInput();
					safetySys.triggerEvent(properties.doMouseControl);			    
				}

			private:
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				DeltaControlSystem& controlSys;


		};
			
		class MouseTimeOutExceptionSequence : public Sequence{
			public:
				MouseTimeOutExceptionSequence(std::string name, Sequencer& sequencer, BaseSequence* caller,DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties):
				Sequence(name, sequencer, caller, true),
				safetySys(safetySys),
				properties(properties){}
				
				int action(){
					safetySys.triggerEvent(properties.doAutoMoving);
				}
				
			private:
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
		};
	}
}
