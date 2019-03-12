#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "step/Move.hpp"
#include "step/Up.hpp"
#include "step/Down.hpp"
#include "step/Grab.hpp"
#include "step/Release.hpp"
#include "step/Emag.hpp"

#include <array>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class MoveBlockSequence : public Sequence {
			public:
				MoveBlockSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys,BaseSequence* caller, SafetySystem& safetySys, Calibration& calibration);
				
				int operator() (int from, int to);
				
				int action();
				
			private:		
				DeltaControlSystem& controlSys;
				SafetySystem& safetySys;
				Move move;
				Up up;
				Down down;
				Grab grab;
				Release release;
				int from;
				int to;
		};
	}
}

