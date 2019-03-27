#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../Calibration.hpp"
#include "step/Move.hpp"


#include <array>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class DetectSequence : public Sequence {
			public:
				DetectSequence(std::string name, DeltaControlSystem& controlSys, Sequence* caller, Calibration& calibration);
				
				int operator() (int pos);
				
				int action();
				
			private:		
				DeltaControlSystem& controlSys;
				Calibration& calibration;
				Move move;
				int position;
				Wait wait;
		};
	}
}

