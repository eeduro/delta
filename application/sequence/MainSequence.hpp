#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "CalibrateBlockSequence.hpp"
#include "AutoMoveSequence.hpp"
#include "HomingSequence.hpp"
#include "ParkSequence.hpp"
#include "MouseSequence.hpp"

#include <array>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class MainSequence : public Sequence {
			public:
				MainSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties properties, Calibration& calibration);
				
				int action();

				
			private:
				HomingSequence homeSeq;
				AutoMoveSequence amSeq;
				ParkSequence parkSeq;
				CalibrateBlockSequence cbSeq;
				MouseSequence mouseSeq;
				
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				uint8_t currentState;
				
				eeros::hal::Input<bool>* buttonGreen;
				eeros::hal::Input<bool>* buttonBlue;
				eeros::hal::Output<bool>* ledBlue;
			};
	}
}

