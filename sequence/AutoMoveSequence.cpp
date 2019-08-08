#include "AutoMoveSequence.hpp"

using namespace eeduro::delta;

AutoMoveSequence::AutoMoveSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration):
	Sequence(name, caller, true),
	sortSeq("Sort Sequence", this, controlSys, properties, calibration),
	shuffSeq("Shuffle Sequence", this, controlSys, properties, calibration),
	circleSeq("Circle Sequence", this, controlSys, safetySys, properties, calibration),
	wait("wait", this),
	moveMouseCondition(controlSys),
	mouseExceptionSequence("Mouse Exception Sequence", this,  safetySys, properties, controlSys, calibration),
	moveMouseMonitor("MouseMoveMonitor", this, moveMouseCondition, SequenceProp::abort, &mouseExceptionSequence),
	safetySys(safetySys),
	properties(properties),
	ec(safetySys, properties),
	emergencyLevel("Emergency Level Monitor", this, ec, eeros::sequencer::SequenceProp::abort),
	emergencyExceptionSequence("Emergency exception sequence", this, controlSys, safetySys, properties)
	
	{ 
		wait.addMonitor(&moveMouseMonitor);
		
		addMonitor(&emergencyLevel);
	}
	


int AutoMoveSequence::action() {
	while(getRunningState() == SequenceState::running){
		log.warn() << getRunningState();
		log.info() << "hello world";
		moveMouseCondition.reset();
		wait(1);
	}
}


