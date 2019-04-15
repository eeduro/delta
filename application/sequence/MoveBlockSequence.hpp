#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../Calibration.hpp"
#include "step/Move.hpp"
#include "step/Grab.hpp"
#include "step/Release.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class MoveBlockSequence : public Sequence {
			public:
				MoveBlockSequence(std::string name, DeltaControlSystem& controlSys, Sequence* caller, Calibration& calibration);
				int operator() (int from, int to);
				int action();
				
			private:		
				DeltaControlSystem& controlSys;
				Calibration& calibration;
				Move move;
				Grab grab;
				Release release;
				int from;
				int to;
		};
	}
}

