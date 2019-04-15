#pragma once

#include <eeros/sequencer/Condition.hpp>
#include <eeros/hal/HAL.hpp>

using namespace eeros::hal;
using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class BlueButtonCondition : public Condition{
			public:
				BlueButtonCondition()
					{
						HAL& hal = HAL::instance();
						buttonBlue = hal.getLogicInput("buttonBlue", false); 
					}

				bool validate() {
					return buttonBlue->get();

				}

				eeros::hal::Input<bool>* buttonBlue;
		};
	}
}