#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "step/Move.hpp"
#include "DetectSequence.hpp"
#include "MoveBlockSequence.hpp"
#include "ExceptionSequence.hpp"
#include "../conditions/MoveMouseCondition.hpp"

#include <array>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class ShuffleSequence : public Sequence {
			public:
				ShuffleSequence(std::string name, Sequencer& sequencer, BaseSequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, Calibration& calibration, DeltaSafetyProperties &properties);
				
				int action();
				
			private:
				virtual int find(const std::array<int,4> &blocks, int block);
				
				DeltaControlSystem& controlSys;
				SafetySystem& safetySys;
				Move move;
				DetectSequence detectSequence;
				MoveBlockSequence moveBlock;
				Calibration& calibration;
		
		};
	}
}

