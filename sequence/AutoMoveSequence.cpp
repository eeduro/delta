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
		
		// ** Sort and shuffle ** //
// 		sortSeq.start();
// 		moveMouseCondition.reset();
// 		wait(5);
// 		shuffSeq.start();
		
		// ** Perform circle movement ** //
		circleSeq.start();
		moveMouseCondition.reset();
		wait(5);
	}
}


