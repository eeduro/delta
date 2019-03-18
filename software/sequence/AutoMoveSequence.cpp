#include "AutoMoveSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include <eeros/sequencer/Sequencer.hpp>
#include "../sequence/ConfigureBlockSequence.hpp"
#include <unistd.h>

using namespace eeduro::delta;

AutoMoveSequence::AutoMoveSequence(std::string name, eeros::sequencer::Sequencer& sequencer, DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, DeltaSafetyProperties properties, Calibration& calibration):
	Sequence(name, sequencer),
	controlSys(controlSys),
	properties(properties),
	calibration(calibration),
	safetySys(safetySys),
	sequencer(sequencer),
	mmc(controlSys),
	mexSeq("Mouse Exception Sequence", sequencer, this,  safetySys, properties, controlSys),
	mouseMove("MouseMoveMonitor", this, mmc, eeros::sequencer::SequenceProp::abort, &mexSeq),
	sortSeq("Sort Sequence", sequencer, this, controlSys, safetySys,calibration, properties),
	shuffSeq("Shuffle Sequence",sequencer, this, controlSys, safetySys, calibration, properties)
	
	{
		mouseMove.setBehavior(eeros::sequencer::SequenceProp::abort);
		addMonitor(&mouseMove);
	}
	


int AutoMoveSequence::action() {
	while(Sequencer::running && safetySys.getCurrentLevel() == properties.slAutoMoving){
		sortSeq.start();
		shuffSeq.start();
	}
}


