#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "step/Move.hpp"
#include "DetectSequence.hpp"
#include "MoveBlockSequence.hpp"
#include "conditions/MoveMouseCondition.hpp"
#include "ExceptionSequence.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class SortSequence : public Sequence {
			public:
				SortSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, DeltaSafetyProperties &properties, Calibration& calibration);
				int action();

			private:
				virtual int find(const std::array<int,4> &blocks, int block);
				
				DeltaControlSystem& controlSys;
				Calibration& calibration;
				Move move;
				DetectSequence detectSequence;
				MoveBlockSequence moveBlock;
		};
	}
}

