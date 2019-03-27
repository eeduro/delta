#include "AutoMoveSequence.hpp"

using namespace eeduro::delta;

AutoMoveSequence::AutoMoveSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties properties, Calibration& calibration):
	Sequence(name, caller, true),
	sortSeq("Sort Sequence", this, controlSys, calibration, properties),
	shuffSeq("Shuffle Sequence", this, controlSys, calibration, properties),
	wait("wait", this),
	mmc(controlSys),
	mexSeq("Mouse Exception Sequence", this,  safetySys, properties, controlSys, calibration),
	mouseMove("MouseMoveMonitor", this, mmc, eeros::sequencer::SequenceProp::abort, &mexSeq),
	ec(safetySys, properties),
	emergencyLevel("Emergency Level Monitor", this, ec, eeros::sequencer::SequenceProp::abort)
	{ 
		mouseMove.setBehavior(eeros::sequencer::SequenceProp::abort);
		wait.addMonitor(&mouseMove);
		addMonitor(&emergencyLevel);
	}
	


int AutoMoveSequence::action() {
	sortSeq.start();
	wait(5);
	shuffSeq.start();
}


