#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>

#include "../../safety/DeltaSafetyProperties.hpp"
#include "../../control/DeltaControlSystem.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class WaitForLevel : public Step {
			public:
				WaitForLevel(std::string name, Sequence* caller, SafetySystem& safetySys) : 
					Step(name, caller), 
					safetySys(safetySys){}
				int operator() (uint32_t level) {this->level = level; return start();}
				int action(){/*empty*/};
				bool checkExitCondition(){
					return safetySys.getCurrentLevel().getLevelId() == level;
				}
			
			private:
				SafetySystem& safetySys;
				uint32_t level;
		};
	}
}