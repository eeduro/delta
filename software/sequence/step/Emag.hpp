#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class Emag : public Step {
			public:
				Emag(std::string name, Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys) : Step(name, seq, caller), controlSys(controlSys){
					this->value = false;
				}
				int operator() (bool value) {this->value = value; return start();}
				int action(){
					controlSys.emagVal.setValue(value);
				};

				DeltaControlSystem &controlSys;
				bool value;
		  };
	}
}