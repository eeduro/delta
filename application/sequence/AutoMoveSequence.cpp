#include "AutoMoveSequence.hpp"

using namespace eeduro::delta;

AutoMoveSequence::AutoMoveSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration):
	Sequence(name, caller, true),
	sortSeq("Sort Sequence", this, controlSys, calibration, properties),
	shuffSeq("Shuffle Sequence", this, controlSys, calibration, properties),
	wait("wait", this),
	moveMouseCondition(controlSys),
	mouseExceptionSequence("Mouse Exception Sequence", this,  safetySys, properties, controlSys, calibration),
	moveMouseMonitor("MouseMoveMonitor", this, moveMouseCondition, SequenceProp::abort, &mouseExceptionSequence),
	safetySys(safetySys),
	properties(properties),
	blueButtonCondition(),
	blueButtonExceptionSequence("Blue button exception sequence", this, controlSys, safetySys, properties, calibration),
	blueButtonMonitor("BlueButtonMonitor", this, blueButtonCondition, SequenceProp::abort, &blueButtonExceptionSequence)
	{ 
		wait.addMonitor(&moveMouseMonitor);
		addMonitor(&blueButtonMonitor);
	}
	


int AutoMoveSequence::action() {
	while(getRunningState() == SequenceState::running){
		log.warn() << getRunningState();
		sortSeq.start();
		moveMouseCondition.reset();
		wait(5);
		shuffSeq.start();
	}
}


