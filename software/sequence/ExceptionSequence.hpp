#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "../Calibration.hpp"

#include <vector>

namespace eeduro {
	namespace delta {
		class MouseExceptionSequence : public eeros::sequencer::Sequence{
		public:
			MouseExceptionSequence(std::string name, eeros::sequencer::Sequencer& sequencer,eeros::sequencer::BaseSequence* caller, eeros::safety::SafetySystem& safetySys, DeltaSafetyProperties& properties, DeltaControlSystem& controlSys):
			Sequence(name, sequencer, caller, true),
			controlSys(controlSys),
			safetySys(safetySys),
			properties(properties),
			sequencer(sequencer)
			{
			  			  
			}
			
			int action(){
			    safetySys.triggerEvent(properties.doMouseControl);			    
			}

			
		private:

			eeros::safety::SafetySystem& safetySys;
			DeltaSafetyProperties& properties;
			DeltaControlSystem& controlSys;
			eeros::sequencer::Sequencer& sequencer;


		};
		
		class MouseTimeOutExceptionSequence : public eeros::sequencer::Sequence{
		public:
			MouseTimeOutExceptionSequence(std::string name, sequencer::Sequencer& sequencer, eeros::sequencer::BaseSequence* caller,DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, DeltaSafetyProperties& properties):
			Sequence(name, sequencer, caller, true),
			safetySys(safetySys),
			properties(properties),
			controlSys(controlSys),
			sequencer(sequencer)
			{
			  
			}
			
			int action(){
			    safetySys.triggerEvent(properties.doAutoMoving);
			}
			
		private:
			eeros::safety::SafetySystem& safetySys;
			DeltaSafetyProperties& properties;
			DeltaControlSystem& controlSys;
			eeros::sequencer::Sequencer& sequencer;
		};
	}
}
