#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "step/Move.hpp"
#include "DetectSequence.hpp"
#include "MoveBlockSequence.hpp"
#include "../conditions/MoveMouseCondition.hpp"
#include "ExceptionSequence.hpp"

#include <array>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class SortSequence : public Sequence {
			public:
				SortSequence(std::string name, Sequencer& sequencer, BaseSequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, Calibration& calibration, DeltaSafetyProperties &properties);
				
				int action();
				
			private:
				virtual int find(const std::array<int,4> &blocks, int block);
				
				DeltaControlSystem& controlSys;
				SafetySystem& safetySys;
				Calibration& calibration;
				Move move;
				DetectSequence detectSequence;
				MoveBlockSequence moveBlock;

		};
	}
}

