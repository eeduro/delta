#include "MouseSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "ExceptionSequence.hpp"

using namespace eeduro::delta;

MouseSequence::MouseSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration) :
	Sequence(name, caller, true),
	controlSys(controlSys),
	safetySys(safetySys),
	grab("grab", this, controlSys),
	release("release", this, controlSys),
	properties(properties),
	mouseTimeoutSequence("Mouse TimeOut Exception Sequence", this, controlSys, safetySys, properties, calibration),
	blueButtonCondition(),
	blueButtonExceptionSequence("Blue button exception sequence", this, controlSys, safetySys, properties, calibration),
	blueButtonMonitor("BlueButtonMonitor", this, blueButtonCondition, SequenceProp::abort, &blueButtonExceptionSequence)
	{
		setTimeoutTime(2.0);
		setTimeoutExceptionSequence(mouseTimeoutSequence);				
		setTimeoutBehavior(SequenceProp::abort);
		controlSys.circlePlanner.setInitPos(controlSys.pathPlanner.getLastPoint());
		mouseNew = controlSys.mouse.getOut().getSignal().getValue();
		mouseOld = mouseNew;
		
		addMonitor(&blueButtonMonitor);
	}

int MouseSequence::action() {
	while(getRunningState() == SequenceState::running){
	
	}
}

