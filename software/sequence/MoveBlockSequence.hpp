#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "step/move.hpp"
#include "step/up.hpp"
#include "step/down.hpp"
#include "step/grab.hpp"
#include "step/release.hpp"
#include "step/emag.hpp"

#include <array>

namespace eeduro {
	namespace delta {
		class MoveBlockSequence : public eeros::sequencer::Sequence {
			public:
				MoveBlockSequence(std::string name, eeros::sequencer::Sequencer& sequencer, DeltaControlSystem& controlSys,BaseSequence* caller, eeros::safety::SafetySystem& safetySys, Calibration& calibration);
				
				int operator() (int from, int to);
				
				int action();
				
			private:		
				DeltaControlSystem& controlSys;
				eeros::safety::SafetySystem& safetySys;
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

