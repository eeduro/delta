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

