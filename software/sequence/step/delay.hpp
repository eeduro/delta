#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"

namespace eeduro{
	namespace delta{
		class Delay : public eeros::sequencer::Step{
			public:
				Delay(std::string name, eeros::sequencer::Sequencer& sequencer, BaseSequence* caller) : Step(name, sequencer, caller){}
				int operator() (double secs) {seconds = secs; return Step::start();}
				
				int action(){
					time = std::chrono::steady_clock::now();
				}
				
				bool checkExitCondition(){
					return ((std::chrono::duration<double>)(std::chrono::steady_clock::now() - time)).count() > seconds;
				}
				
			private:
				std::chrono::time_point<std::chrono::steady_clock> time;
				double seconds;
			
		};
	}
}