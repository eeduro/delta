#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "SortSequence.hpp"
#include "ShuffleSequence.hpp"
#include "CircleSequence.hpp"
#include "ExceptionSequence.hpp"
#include "conditions/BlueButtonCondition.hpp"
#include "conditions/EmergencyCondition.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class AutoMoveSequence : public Sequence {
			public:
				AutoMoveSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration);
				int action();
				
			private:
				SortSequence sortSeq;
				ShuffleSequence shuffSeq;
				CircleSequence circleSeq;
				Wait wait;
				
				MouseExceptionSequence mouseExceptionSequence;
				MoveMouseCondition moveMouseCondition;
				Monitor moveMouseMonitor;
				
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				
				BlueButtonExceptionSequence blueButtonExceptionSequence;
				BlueButtonCondition blueButtonCondition;
				Monitor blueButtonMonitor;
			};
	}
}

