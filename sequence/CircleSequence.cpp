#include "CircleSequence.hpp"

using namespace eeduro::delta;

CircleSequence::CircleSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration):
Sequence(name, caller, true),
	wait("wait", this),
	controlSys(controlSys),
	safetySys(safetySys),
	properties(properties),
	blueButtonCondition(),
	blueButtonExceptionSequence("Blue button exception sequence", this, controlSys, safetySys, properties, calibration),
	blueButtonMonitor("BlueButtonMonitor", this, blueButtonCondition, SequenceProp::abort, &blueButtonExceptionSequence)
	{ 
		controlSys.circlePlanner.setInitPos(controlSys.pathPlanner.getLastPoint());
		
		addMonitor(&blueButtonMonitor);
	}


int CircleSequence::action() {
	while(getRunningState() == SequenceState::running){
// 		log.warn() << getRunningState();
	  	wait(2);
	}
}
