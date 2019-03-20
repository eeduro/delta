#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../Calibration.hpp"
#include "step/Wait.hpp"
#include "step/Move.hpp"


#include <array>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class DetectSequence : public Sequence {
			public:
				DetectSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys,BaseSequence* caller, SafetySystem& safetySys, Calibration& calibration);
				
				int operator() (int pos);
				
				int action();
				
				int getBlock();
				
			private:		
				DeltaControlSystem& controlSys;
				SafetySystem& safetySys;
				Calibration& calibration;
				Move move;
				int position;
				int blockNumber;
				Wait wait;
		};
	}
}

