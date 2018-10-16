#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "step/move.hpp"
#include "step/detect.hpp"
#include "MoveBlockSequence.hpp"
#include "ExceptionSequence.hpp"
#include "step/emag.hpp"

#include <array>

namespace eeduro {
	namespace delta {
		class MouseSequence : public eeros::sequencer::Sequence {
		public:
			MouseSequence(std::string name, eeros::sequencer::Sequencer& sequencer, DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration);
			
			int action();

		private:
			int count;
			DeltaControlSystem& controlSys;
			eeros::safety::SafetySystem& safetySys;
			bool buttonPressed = false;
			AxisVector mouseNew;
			AxisVector mouseOld;
			MouseTimeOutExceptionSequence mousetimeoutSeq;
			Emag emag;
		};
	}
}
