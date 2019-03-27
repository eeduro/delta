#include "MainSequence.hpp"

using namespace eeduro::delta;

MainSequence::MainSequence(std::string name, eeros::sequencer::Sequencer& sequencer, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties properties, Calibration& calibration):
	Sequence(name, sequencer),
	properties(properties),
	safetySys(safetySys),
	homeSeq("Home Sequence", this, controlSys, safetySys, properties, calibration),
	amSeq("AutoMove Sequence", this, controlSys, safetySys, properties, calibration),
	parkSeq("Park Sequence", this, controlSys, safetySys, properties, calibration),
	cbSeq("Calibrate Block Sequence", this, controlSys, safetySys, properties, calibration),
	mouseSeq("Mouse Sequence", sequencer, controlSys, safetySys, properties, calibration),
	currentState(0)
	{ 
		HAL& hal = HAL::instance();
		buttonGreen = hal.getLogicInput("buttonGreen", false);
		buttonBlue = hal.getLogicInput("buttonBlue", false);
		ledBlue = hal.getLogicOutput("ledBlue", false);
	}
	


int MainSequence::action() {
	
	while(Sequencer::running){
		
		if(safetySys.getCurrentLevel() == properties.slEmergency){
			/* do nothing */
		}else if(safetySys.getCurrentLevel() == properties.slHoming){
			homeSeq.start();
		}else if(safetySys.getCurrentLevel() == properties.slSystemReady){
			if(buttonGreen->get()) safetySys.triggerEvent(properties.doAutoMoving);
			if(buttonBlue->get()) cbSeq.start();
		}else if(safetySys.getCurrentLevel() == properties.slAutoMoving){
			if(amSeq.getRunningState() != SequenceState::running){
				amSeq.start();
			}
		}else if(safetySys.getCurrentLevel() == properties.slMouseControl){
			if(mouseSeq.getRunningState() != SequenceState::running){
				mouseSeq.start();
			}
		}else if(safetySys.getCurrentLevel() == properties.slParking){
			if(parkSeq.getRunningState() != SequenceState::running){
				parkSeq.start();
				break;
			}
		}

	}
	
	
	
	
}


