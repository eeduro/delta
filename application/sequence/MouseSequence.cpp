#include "MouseSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "ExceptionSequence.hpp"

#include <unistd.h>

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
		controlSys.mouse.setInitPos(controlSys.pathPlanner.getLastPoint());
		mouseNew = controlSys.mouse.getOut().getSignal().getValue();
		mouseOld = mouseNew;
		
		addMonitor(&blueButtonMonitor);
	}
	

      


int MouseSequence::action() {
	while(getRunningState() == SequenceState::running){
		mouseNew = controlSys.mouse.getOut().getSignal().getValue();
		if(controlSys.mouse.getButtonOut().getSignal().getValue()[0] || controlSys.mouse.getButtonOut().getSignal().getValue()[2]){
			buttonPressed = true;
			grab();
		}
		else{
			buttonPressed = false;
			release();
		}
		
		if(buttonPressed || mouseNew != mouseOld){
			resetTimeout();
		}
		mouseOld = mouseNew;
	}
}

bool MouseSequence::mouseMoved(){
	if(mouseNew[0] == mouseOld[0] && mouseNew[1] == mouseOld[1] && mouseNew[2] == mouseOld[2]){
		return false;
	}
	else{
		return true;
	}
}

