#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "conditions/EmergencyCondition.hpp"
#include "../control/DeltaControlSystem.hpp"
#include "step/Move.hpp"
#include "MoveBlockSequence.hpp"
#include "ExceptionSequence.hpp"
#include "step/Grab.hpp"
#include "step/Release.hpp"
#include "conditions/BlueButtonCondition.hpp"

#include <array>

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class MouseSequence : public Sequence {
			public:
				MouseSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration);
				
				int action();
				bool mouseMoved();

			private:
				//int count;
				DeltaControlSystem& controlSys;
				DeltaSafetyProperties& properties;
				SafetySystem& safetySys;
				bool buttonPressed = false;
				AxisVector mouseNew;
				AxisVector mouseOld;
				MouseTimeOutExceptionSequence mouseTimeoutSequence;
				Grab grab;
				Release release;
				
				BlueButtonExceptionSequence blueButtonExceptionSequence;
				BlueButtonCondition blueButtonCondition;
				Monitor blueButtonMonitor;
		};
	}
}
