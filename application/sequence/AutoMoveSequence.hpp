#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "../conditions/MoveMouseCondition.hpp"
#include "ConfigureBlockSequence.hpp"
#include "SortSequence.hpp"
#include "ShuffleSequence.hpp"
#include "ExceptionSequence.hpp"

#include <array>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class AutoMoveSequence : public Sequence {
			public:
				AutoMoveSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties properties, Calibration& calibration);
				
				int action();

				
			private:
				
				DeltaControlSystem& controlSys;
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				
				Calibration& calibration;

				Sequencer& sequencer;
				
				MouseExceptionSequence mexSeq;
				MoveMouseCondition mmc;
				Monitor mouseMove;
				
				SortSequence sortSeq;
				ShuffleSequence shuffSeq;
			};
	}
}

