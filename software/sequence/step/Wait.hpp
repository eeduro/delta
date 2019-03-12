#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/BaseSequence.hpp>

#include "../../control/DeltaControlSystem.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class Wait : public Step{
			public:
				Wait(std::string name, Sequencer& sequencer, BaseSequence* caller) : Step(name, sequencer, caller){}
				int operator() (double time) {seconds = time; return start();}
				
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