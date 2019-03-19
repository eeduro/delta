#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"
#include "../../Calibration.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class Release : public Step {
			public:
				Release(std::string name, Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys) : 
					Step(name, seq, caller), controlSys(controlSys){
				}
				int action(){
					controlSys.emagVal.setValue(false);
				};
		      
				DeltaControlSystem &controlSys;
		  };
	}
}