#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"
#include "../../Calibration.hpp"
#include "emag.hpp"

namespace eeduro{
	namespace delta{
		class Release : public eeros::sequencer::Step {
			public:
				Release(std::string name,eeros::sequencer::Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys) : 
					Step(name, seq, caller), controlSys(controlSys), emag("Set Elektromagnet", seq, caller, controlSys){
				}
				int operator() () {return Step::start();}
				int action(){
					emag(false);
				};
		      
				DeltaControlSystem &controlSys;
				Emag emag;
		  };
	}
}