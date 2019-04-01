#include "MouseSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "ExceptionSequence.hpp"

#include <unistd.h>

using namespace eeduro::delta;

MouseSequence::MouseSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration) :
	Sequence(name, sequencer),
	controlSys(controlSys),
	safetySys(safetySys),
	grab("grab", this, controlSys),
	release("release", this, controlSys),
	properties(properties),
	mouseTimeoutSequence("Mouse TimeOut Exception Sequence", this, controlSys, safetySys, properties, calibration),
	ec(safetySys, properties),
	emergencyLevel("Emergency Level Monitor", this, ec, SequenceProp::abort)
	{
		setTimeoutTime(2.0);
		setTimeoutExceptionSequence(mouseTimeoutSequence);				
		setTimeoutBehavior(SequenceProp::abort);
		controlSys.mouse.setInitPos(controlSys.pathPlanner.getLastPoint());
		mouseNew = controlSys.mouse.getOut().getSignal().getValue();
		mouseOld = mouseNew;
		count = 0;
		
		addMonitor(&emergencyLevel);
	}
	

      


int MouseSequence::action() {
		mouseNew = controlSys.mouse.getOut().getSignal().getValue();
		log.warn() << controlSys.directKin.getOut().getSignal().getValue();
		if(controlSys.mouse.getButtonOut().getSignal().getValue()[0] || controlSys.mouse.getButtonOut().getSignal().getValue()[1]){
			buttonPressed = true;
			grab();
		}
		else{
			buttonPressed = false;
			release();
		}
		
		while(!buttonPressed && mouseNew == mouseOld && count < (2000)){
			mouseNew = controlSys.mouse.getOut().getSignal().getValue();
			release();
			count++;
		}
		
		count = 0;
		mouseOld = mouseNew;
}

bool MouseSequence::mouseMoved(){
	if(mouseNew[0] == mouseOld[0] && mouseNew[1] == mouseOld[1] && mouseNew[2] == mouseOld[2]){
		return false;
	}
	else{
		return true;
	}
}

