#pragma once

#include <eeros/sequencer/Condition.hpp>
#include "../../safety/DeltaSafetyProperties.hpp"
#include <eeros/safety/SafetySystem.hpp>


using namespace eeros::sequencer;
namespace eeduro{
	namespace delta{
    
		class EmergencyCondition : public Condition{
			public:
				EmergencyCondition(SafetySystem& safetySys, DeltaSafetyProperties& properties) : 
					safetySys(safetySys),properties(properties)
					{ /*empty*/ }

				bool validate() {
					return safetySys.getCurrentLevel() == properties.slEmergency;

				}

				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				
		};
	}
}