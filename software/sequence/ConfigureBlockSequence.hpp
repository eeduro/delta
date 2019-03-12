#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "MouseSequence.hpp"
#include "../Calibration.hpp"

#include <vector>

using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace eeros::hal;

namespace eeduro {
	namespace delta {
		class ConfigureBlockSequence : public Sequence{
			public:
				ConfigureBlockSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys, SafetySystem& safetySys, Calibration& calibration);
				
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
				
				eeros::hal::Input<bool>* buttonBlue;			
				eeros::hal::Output<bool>* ledBlue;

		};
	}
}
