#include "MouseSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "ExceptionSequence.hpp"

#include <unistd.h>

using namespace eeduro::delta;

MouseSequence::MouseSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration) :
	Sequence(name, sequencer),
	controlSys(controlSys),
	safetySys(safetySys),
	grab("grab", sequencer, this, controlSys),
	release("release", sequencer, this, controlSys),
	properties(properties),
	mousetimeoutSeq("Mouse TimeOut Exception Sequence", sequencer, this, controlSys, safetySys, properties){
		setTimeoutTime(2.0);
		setTimeoutExceptionSequence(mousetimeoutSeq);				
		setTimeoutBehavior(SequenceProp::abort);
		controlSys.mouse.setInitPos(controlSys.pathPlanner.getLastPoint());
		mouseNew = controlSys.mouse.getOut().getSignal().getValue();
		mouseOld = mouseNew;
		count = 0;
	}
	

      


int MouseSequence::action() {
		mouseNew = controlSys.mouse.getOut().getSignal().getValue();
		log.warn() << controlSys.directKin.getOut().getSignal().getValue();
		if(controlSys.mouse.getButtonOut().getSignal().getValue()[0]){
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

