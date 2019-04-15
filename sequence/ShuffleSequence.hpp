#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "step/Move.hpp"
#include "DetectSequence.hpp"
#include "MoveBlockSequence.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class ShuffleSequence : public Sequence {
			public:
				ShuffleSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, Calibration& calibration, DeltaSafetyProperties &properties);
				int action();

			private:
				virtual int find(const std::array<int,4> &blocks, int block);
				
				DeltaControlSystem& controlSys;
				Move move;
				DetectSequence detectSequence;
				MoveBlockSequence moveBlock;
				Calibration& calibration;
		};
	}
}

