#pragma once

#include <vector>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "MouseSequence.hpp"
#include "../Calibration.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace eeros::hal;

namespace eeduro {
	namespace delta {
		class CalibrateBlockSequence : public Sequence{
			public:
				CalibrateBlockSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration);
				
				virtual int action();

				bool getDone();
				
			protected:
				void waitUntilReady();
				void waitForButton(std::vector<int> buttons);
				void logAndWaitForButton(std::vector<int> buttons);
				void waitForGreenButton();
				void waitForRedButton();
				void waitForBlueButton();
				void waitForBlueOrRedButton();
				
				bool done = false;
				
			private:
				Calibration& calibration;
				DeltaControlSystem& controlSys;
				DeltaSafetyProperties& safetyProp;
				SafetySystem& safetySys;
				
				Wait wait;
				
				eeros::hal::Input<bool>* buttonBlue;
				eeros::hal::Output<bool>* ledBlue;
		};
	}
}
