#pragma once

#include <eeros/sequencer/Condition.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../../safety/DeltaSafetyProperties.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class EmergencyCondition : public Condition{
		public:
			EmergencyCondition(SafetySystem& safetySys, DeltaSafetyProperties& safetyProp) : safetySys(safetySys), safetyProp(safetyProp) {  }
			bool validate() {
				return safetySys.getCurrentLevel() == safetyProp.slEmergency;
			}

		private:
			SafetySystem& safetySys;
			DeltaSafetyProperties& safetyProp;
		};
	}
}