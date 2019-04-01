#include "MainSequence.hpp"

using namespace eeduro::delta;

MainSequence::MainSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties properties, Calibration& calibration):
	Sequence(name, sequencer),
	properties(properties),
	safetySys(safetySys),
	controlSys(controlSys),
	homeSeq("Home Sequence", this, controlSys, safetySys, properties, calibration),
	amSeq("AutoMove Sequence", this, controlSys, safetySys, properties, calibration),
	parkSeq("Park Sequence", this, controlSys, safetySys, properties, calibration),
	cbSeq("Calibrate Block Sequence", this, controlSys, safetySys, properties, calibration),
	mouseSeq("Mouse Sequence", sequencer, controlSys, safetySys, properties, calibration),
	wait("Wait", this)
	{ 
		HAL& hal = HAL::instance();
		buttonGreen = hal.getLogicInput("buttonGreen", false);
		buttonBlue = hal.getLogicInput("buttonBlue", false);
		ledBlue = hal.getLogicOutput("ledBlue", false);
	}
	


int MainSequence::action() {
	
	while(Sequencer::running){
		
		if(safetySys.getCurrentLevel() == properties.slEmergency){
			//Sequencer::instance().abort();
			if(buttonGreen->get()){
				safetySys.triggerEvent(properties.doControlStart);
			}
			if(safetySys.getCurrentLevel().getNofActivations() > 5000){
				controlSys.start();
				controlSys.enableAxis();
				safetySys.triggerEvent(properties.doParking);
			}
		}else if(safetySys.getCurrentLevel() == properties.slSystemOn){
			safetySys.triggerEvent(properties.doPoweringUp);
		}else if(safetySys.getCurrentLevel() == properties.slPoweringUp){
			safetySys.triggerEvent(properties.doHoming);
		}else if(safetySys.getCurrentLevel() == properties.slHoming){
			homeSeq.start();
			safetySys.triggerEvent(properties.homingDone);
		}else if(safetySys.getCurrentLevel() == properties.slHomed){
			safetySys.triggerEvent(properties.doSystemReady);
		}else if(safetySys.getCurrentLevel() == properties.slSystemReady){
			if(buttonGreen->get()) safetySys.triggerEvent(properties.doAutoMoving);
			if(buttonBlue->get()) cbSeq.start();
		}else if(safetySys.getCurrentLevel() == properties.slAutoMoving){
			amSeq.start();
		}else if(safetySys.getCurrentLevel() == properties.slMouseControl){
			mouseSeq.start();
		}else if(safetySys.getCurrentLevel() == properties.slParking){
			parkSeq.start();
		}else if(safetySys.getCurrentLevel() == properties.slParked){
			safetySys.triggerEvent(properties.doControlStop);
			break;
		}
		
		wait(0.1);

	}
	
	
	
	
}


