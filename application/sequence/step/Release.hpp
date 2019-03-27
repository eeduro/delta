#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>

#include "../../control/DeltaControlSystem.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class Release : public Step {
			public:
				Release(std::string name, Sequence* caller, DeltaControlSystem& controlSys) : 
					Step(name, caller), controlSys(controlSys){
				}
				int action(){
					controlSys.emagVal.setValue(false);
				};
		      
				DeltaControlSystem &controlSys;
		  };
	}
}