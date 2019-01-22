#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "MouseSequence.hpp"
#include "../Calibration.hpp"

#include <vector>

namespace eeduro {
	namespace delta {
		class CalibrateSequence : public eeros::sequencer::Sequence{
			public:
				CalibrateSequence(std::string name, eeros::sequencer::Sequencer& sequencer, DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, Calibration& calibration);
				
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
				eeduro::delta::DeltaControlSystem& controlSys;
				eeduro::delta::DeltaSafetyProperties& safetyProp;
				eeros::safety::SafetySystem& safetySys;
				
				eeros::hal::Input<bool>* buttonBlue;			
				eeros::hal::Output<bool>* ledBlue;

		};
	}
}
