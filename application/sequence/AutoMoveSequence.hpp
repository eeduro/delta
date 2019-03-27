#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>

#include "../conditions/EmergencyCondition.hpp"
#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "SortSequence.hpp"
#include "ShuffleSequence.hpp"
#include <eeros/sequencer/Wait.hpp>

#include <array>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class AutoMoveSequence : public Sequence {
			public:
				AutoMoveSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties properties, Calibration& calibration);
				
				int action();

				
			private:
				SortSequence sortSeq;
				ShuffleSequence shuffSeq;
				Wait wait;
				
				MouseExceptionSequence mexSeq;
				MoveMouseCondition mmc;
				Monitor mouseMove;
				
				EmergencyCondition ec;
				Monitor emergencyLevel;
			};
	}
}

