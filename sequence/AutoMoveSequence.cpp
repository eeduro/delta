#include "AutoMoveSequence.hpp"

using namespace eeduro::delta;

AutoMoveSequence::AutoMoveSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration):
	Sequence(name, caller, true),
	controlSys(controlSys),
	safetySys(safetySys),
	properties(properties),
// 	sortSeq("Sort Sequence", this, controlSys, properties, calibration),
// 	shuffSeq("Shuffle Sequence", this, controlSys, properties, calibration),
	circleSeq("Circle Sequence", this, controlSys, safetySys, properties, calibration),
	wait("wait", this),
	moveMouseCondition(controlSys),
	mouseExceptionSequence("Mouse Exception Sequence", this,  safetySys, properties, controlSys, calibration),
	moveMouseMonitor("MouseMoveMonitor", this, moveMouseCondition, SequenceProp::abort, &mouseExceptionSequence)
	{ 
		wait.addMonitor(&moveMouseMonitor);
	}


int AutoMoveSequence::action() {
	while(getRunningState() == SequenceState::running){
		log.warn() << getRunningState();
		moveMouseCondition.reset();
		wait(1);
	}
}


