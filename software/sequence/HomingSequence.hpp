#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "../Calibration.hpp"

#include "step/Home.hpp"

#include <vector>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro{
	namespace delta{
		class HomingSequence : public Sequence{
			public:
				HomingSequence(std::string name, Sequencer& seq, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration );
				int action();
								
			private:
				DeltaControlSystem& controlSys;
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				
				Home home;
				
			
		};
	}
}