#include "AutoMoveSequence.hpp"

using namespace eeduro::delta;

AutoMoveSequence::AutoMoveSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties properties, Calibration& calibration):
	Sequence(name, caller, true),
	sortSeq("Sort Sequence", this, controlSys, calibration, properties),
	shuffSeq("Shuffle Sequence", this, controlSys, calibration, properties),
	wait("wait", this),
	moveMouseCondition(controlSys),
	mouseExceptionSequence("Mouse Exception Sequence", this,  safetySys, properties, controlSys, calibration),
	moveMouseMonitor("MouseMoveMonitor", this, moveMouseCondition, SequenceProp::abort, &mouseExceptionSequence),
	ec(safetySys, properties),
	emergencyLevel("Emergency Level Monitor", this, ec, SequenceProp::abort)
	{ 
		moveMouseMonitor.setBehavior(SequenceProp::abort);
		wait.addMonitor(&moveMouseMonitor);
		addMonitor(&emergencyLevel);
	}
	


int AutoMoveSequence::action() {
	sortSeq.start();
	wait(5);
	shuffSeq.start();
}


