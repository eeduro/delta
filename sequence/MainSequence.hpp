#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "CalibrateBlockSequence.hpp"
#include "AutoMoveSequence.hpp"
#include "HomingSequence.hpp"
#include "ParkSequence.hpp"
#include "MouseSequence.hpp"
#include "step/WaitForLevel.hpp"

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
				
				DeltaControlSystem& controlSys;
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				
				Wait wait;
				WaitForLevel waitForLevel;
				
				EmergencyCondition ec;
				Monitor emergencyLevel;
				
				eeros::hal::Input<bool>* buttonGreen;
				eeros::hal::Input<bool>* buttonBlue;
				eeros::hal::Output<bool>* ledBlue;
				
				uint8_t blueButtonCounter = 0;
			};
	}
}

