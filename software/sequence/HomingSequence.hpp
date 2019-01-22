#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"

#include "step/raise.hpp"
#include "step/home.hpp"

#include <vector>

namespace eeduro{
	namespace delta{
		class HomingSequence : public eeros::sequencer::Sequence{
			public:
				HomingSequence(std::string name, sequencer::Sequencer& seq, DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, DeltaSafetyProperties& properties);
				int action();
								
			private:
				DeltaControlSystem& controlSys;
				eeros::safety::SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				
				Raise raise;
				Home home;
				
			
		};
	}
}