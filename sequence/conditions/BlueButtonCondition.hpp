#pragma once

#include <eeros/sequencer/Condition.hpp>
#include <eeros/hal/HAL.hpp>

using namespace eeros::hal;
using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class BlueButtonCondition : public Condition {
		public:
			BlueButtonCondition() {buttonBlue = HAL::instance().getLogicInput("buttonBlue", false);}
			bool validate() {return buttonBlue->get();}
		private:
			eeros::hal::Input<bool>* buttonBlue;
		};
	}
}