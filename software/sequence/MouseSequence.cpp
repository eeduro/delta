#include "MouseSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "ExceptionSequence.hpp"

#include <unistd.h>

using namespace eeduro::delta;
using namespace eeros::sequencer;
using namespace eeros::safety;
MouseSequence::MouseSequence(std::string name, eeros::sequencer::Sequencer& sequencer, DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration) :
	Sequence(name, sequencer),
	controlSys(controlSys),
	safetySys(safetySys),
	emag("Set Elektromagnet", sequencer, this, controlSys),
	mousetimeoutSeq("Mouse TimeOut Exception Sequence", sequencer, this, controlSys, safetySys, properties){
	  setTimeoutTime(2.0);
	  setTimeoutExceptionSequence(mousetimeoutSeq);				
	  setTimeoutBehavior(eeros::sequencer::SequenceProp::abort);
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
	    emag(true);
	}
	else{
	    buttonPressed = false;
	    emag(false);
	}
	
	while(!buttonPressed && mouseNew == mouseOld && count < (2000)){
	 mouseNew = controlSys.mouse.getOut().getSignal().getValue();
	 emag(false);
	 count++;
	}
	
	count = 0;
	
	mouseOld = mouseNew;
      
}
