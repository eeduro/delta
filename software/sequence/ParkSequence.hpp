#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"

#include "step/Move.hpp"
#include "step/Up.hpp"
#include "step/Down.hpp"
#include "step/Release.hpp"
#include "step/Wait.hpp"

#include <vector>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro{
	namespace delta{
		class ParkSequence : public Sequence{
			public:
				ParkSequence(std::string name, Sequencer& seq, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration);
				int action();
								
			private:
				DeltaControlSystem& controlSys;
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				
				Release release;
				Up up;
				Move move;
				Down down;
				Wait wait;
				
			
		};
	}
}