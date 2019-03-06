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
	properties(properties),
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
	//while(safetySys.getCurrentLevel() == properties.slMouseControl){
		mouseNew = controlSys.mouse.getOut().getSignal().getValue();
		log.warn() << controlSys.directKin.getOut().getSignal().getValue();
		//log.error() << controlSys.mouse.getOut().getSignal();
		log.error() << "x: " << (mouseNew[0] - mouseOld[0]) << "\ty: " << (mouseNew[1]-mouseOld[1]) << "\tphi: " << (mouseNew[2]-mouseOld[2]);
		if(controlSys.mouse.getButtonOut().getSignal().getValue()[0]){
			log.info() << "Mousebutton pressed";
			buttonPressed = true;
			emag(true);
		}
		else{
			log.info() << "Mousebutton not pressed";
			buttonPressed = false;
			emag(false);
		}
		
		while(!buttonPressed && mouseNew == mouseOld && count < (2000)){
			log.error() << "counting: " << count;
			mouseNew = controlSys.mouse.getOut().getSignal().getValue();
			emag(false);
			count++;
		}
		
		count = 0;
		
		mouseOld = mouseNew;
	//}
}

bool MouseSequence::mouseMoved(){
	if(mouseNew[0] == mouseOld[0] && mouseNew[1] == mouseOld[1] && mouseNew[2] == mouseOld[2]){
		log.info() << "mouse not moved";
		return false;
	}
	else{
		return true;
	}
}

