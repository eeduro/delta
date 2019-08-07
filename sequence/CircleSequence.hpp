#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>
#include <eeros/sequencer/Wait.hpp>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "SortSequence.hpp"
#include "ShuffleSequence.hpp"
#include "ExceptionSequence.hpp"
#include "conditions/BlueButtonCondition.hpp"
#include "conditions/EmergencyCondition.hpp"


using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class CircleSequence : public Sequence {
			public:
				CircleSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration);
				int action();
				
			private:
				Wait wait;
			
				DeltaControlSystem& controlSys;
				DeltaSafetyProperties& properties;
				SafetySystem& safetySys;
				
				BlueButtonExceptionSequence blueButtonExceptionSequence;
				BlueButtonCondition blueButtonCondition;
				Monitor blueButtonMonitor;
			};
	}
}

