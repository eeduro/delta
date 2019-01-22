#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "../conditions/MoveMouseCondition.hpp"
#include "CalibrateSequence.hpp"
#include "SortSequence.hpp"
#include "ShuffleSequence.hpp"
#include "ExceptionSequence.hpp"

#include <array>

namespace eeduro {
	namespace delta {
		class AutoMoveSequence : public eeros::sequencer::Sequence {
			public:
				AutoMoveSequence(std::string name, eeros::sequencer::Sequencer& sequencer, DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, DeltaSafetyProperties properties, Calibration& calibration);
				
				int action();

				
			private:
				
				DeltaControlSystem& controlSys;
				eeros::safety::SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				
				Calibration& calibration;

				eeros::sequencer::Sequencer& sequencer;
				
				MouseExceptionSequence mexSeq;
				MoveMouseCondition mmc;
				eeros::sequencer::Monitor mouseMove;
				
				SortSequence sortSeq;
				ShuffleSequence shuffSeq;
			};
	}
}

