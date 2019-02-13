#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"

#include "step/move.hpp"
#include "step/up.hpp"
#include "step/down.hpp"
#include "step/release.hpp"
#include "step/delay.hpp"

#include <vector>

namespace eeduro{
	namespace delta{
		class ParkSequence : public eeros::sequencer::Sequence{
			public:
				ParkSequence(std::string name, sequencer::Sequencer& seq, DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration);
				int action();
								
			private:
				DeltaControlSystem& controlSys;
				eeros::safety::SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				
				Release release;
				Up up;
				Move move;
				Down down;
				Delay delay;
				
			
		};
	}
}