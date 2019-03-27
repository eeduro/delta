#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "../Calibration.hpp"

#include "step/Move.hpp"
#include "step/Release.hpp"


#include <vector>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro{
	namespace delta{
		class ParkSequence : public Sequence{
			public:
				ParkSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration);
				int action();
								
			private:
				DeltaControlSystem& controlSys;
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				Calibration& calibration;
				
				Release release;
				Move move;
				Wait wait;
				
			
		};
	}
}