#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "step/Move.hpp"
#include "step/Detect.hpp"
#include "MoveBlockSequence.hpp"
#include "../conditions/MoveMouseCondition.hpp"
#include "ExceptionSequence.hpp"

#include <array>

namespace eeduro {
	namespace delta {
		class SortSequence : public eeros::sequencer::Sequence {
			public:
				SortSequence(std::string name, eeros::sequencer::Sequencer& sequencer,eeros::sequencer::BaseSequence* caller, DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, Calibration& calibration, DeltaSafetyProperties &properties);
				
				int action();
				
			private:
				virtual void sortBlocks(std::array<int,4> blocks);
				virtual int find(const std::array<int,4> &blocks, int block);
				
				DeltaControlSystem& controlSys;
				eeros::safety::SafetySystem& safetySys;
				Move move;
				Detect detect;
				MoveBlockSequence moveBlock;

		};
	}
}

