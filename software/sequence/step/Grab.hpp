#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"
#include "Emag.hpp"


namespace eeduro{
	namespace delta{
		class Grab : public eeros::sequencer::Step {
			public:
				Grab(std::string name,eeros::sequencer::Sequencer & seq, BaseSequence* caller, DeltaControlSystem& controlSys) : 
					Step(name, seq, caller), controlSys(controlSys), emag("Set Elektromagnet", seq, caller, controlSys){
				}
				int operator() () {return Step::start();}
				int action(){
					emag(true);
				};
		      
				DeltaControlSystem &controlSys;
				Emag emag; 
		  };
	}
}