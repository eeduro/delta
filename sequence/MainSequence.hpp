#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "CircleSequence.hpp"
#include "HomingSequence.hpp"
#include "ParkSequence.hpp"
#include "MouseSequence.hpp"
#include "step/WaitForLevel.hpp"
#include "ExceptionSequence.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class MainSequence : public Sequence {
			public:
				MainSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& safetyProp);
				int action();
				
			private:
				HomingSequence homingSeq;
				CircleSequence circleSeq;
				ParkSequence parkSeq;
				MouseSequence mouseSeq;
				Wait wait;
				WaitForLevel waitForLevel;
				
				EmergencyExceptionSequence emergencyExceptionSeq;
				EmergencyCondition emergencyCondition;
				Monitor emergencyMonitor;
				
				eeros::hal::Input<bool>* buttonGreen;
				eeros::hal::Input<bool>* buttonBlue;
				eeros::hal::Output<bool>* ledBlue;
				DeltaControlSystem& controlSys;
				SafetySystem& safetySys;
				DeltaSafetyProperties& safetyProp;
				uint8_t blueButtonCounter = 0;
			};
	}
}

