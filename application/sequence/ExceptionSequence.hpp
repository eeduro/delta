#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "../Calibration.hpp"
#include "step/Move.hpp"

#include <vector>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class MouseExceptionSequence : public Sequence{
			public:
				
				MouseExceptionSequence(std::string name, BaseSequence* caller, SafetySystem& safetySys, DeltaSafetyProperties& properties, DeltaControlSystem& controlSys, Calibration& calibration):
				Sequence(name, caller, true),
				controlSys(controlSys),
				safetySys(safetySys),
				properties(properties),
				move("move", this, controlSys),
				calibration(calibration){}
				
				int action(){
					move({ 0, 0, calibration.transportation_height, 0});
					controlSys.setMouseInput();
					safetySys.triggerEvent(properties.doMouseControl);			    
				}

			private:
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				DeltaControlSystem& controlSys;
				Calibration& calibration;
				Move move;


		};
			
		class MouseTimeOutExceptionSequence : public Sequence{
			public:
				MouseTimeOutExceptionSequence(std::string name, Sequence* caller,DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration):
				Sequence(name, caller, true),
				safetySys(safetySys),
				properties(properties),
				calibration(calibration),
				move("move", this, controlSys){}
				
				int action(){
					move({ 0, 0, calibration.transportation_height, 0});
					safetySys.triggerEvent(properties.doAutoMoving);
				}
				
			private:
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				Calibration& calibration;
				Move move;
		};
	}
}
