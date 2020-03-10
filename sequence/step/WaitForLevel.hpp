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
				WaitForLevel(std::string name, Sequence* caller, SafetySystem& safetySys) : Step(name, caller), safetySys(safetySys) { }
				int operator() (SafetyLevel& level) {this->level = &level; return start();}
				int action() { };
				bool checkExitCondition() {return safetySys.getCurrentLevel() == *level;}
			
			private:
				SafetySystem& safetySys;
				SafetyLevel* level;
		};
	}
}