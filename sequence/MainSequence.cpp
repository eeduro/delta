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
	mouseSeq("Mouse Sequence", this, controlSys, safetySys, properties, calibration),
	wait("Wait main sequence", this),
	waitForLevel("Wait for level", this, safetySys),
	ec(safetySys, properties),
	emergencyLevel("Emergency Level Monitor", this, ec, eeros::sequencer::SequenceProp::restart),
	emergencyExceptionSequence("Emergency exception sequence", this, controlSys, safetySys, properties)
	{ 
		HAL& hal = HAL::instance();
		buttonGreen = hal.getLogicInput("buttonGreen", false);
		buttonBlue = hal.getLogicInput("buttonBlue", false);
		ledBlue = hal.getLogicOutput("ledBlue", false);
		
		addMonitor(&emergencyLevel);
	}
	
int MainSequence::action() {
	
	ledBlue->set(false);
	waitForLevel(properties.slSystemOn.getLevelId());
	safetySys.triggerEvent(properties.doPoweringUp);
	waitForLevel(properties.slPoweringUp.getLevelId());
	safetySys.triggerEvent(properties.doHoming);
	waitForLevel(properties.slHoming.getLevelId());
	homeSeq.start();
	waitForLevel(properties.slHomed.getLevelId());
	safetySys.triggerEvent(properties.doSystemReady);
	waitForLevel(properties.slSystemReady.getLevelId());
	
	ledBlue->set(true);
	
	while(getRunningState() == SequenceState::running){				// slSystemReady, green button will start automove, green&blue for calibration, 3x blue (or 3 seconds) to park
		
		if(safetySys.getCurrentLevel() == properties.slSystemReady){
			log.error() << buttonGreen->get();
			
			if(buttonGreen->get()){
				safetySys.triggerEvent(properties.doAutoMoving);     // go to auto moving
				waitForLevel(properties.slAutoMoving.getLevelId());
			}
			else{
			  safetySys.triggerEvent(properties.doMouseControl);    // go to mouse control
			  waitForLevel(properties.slMouseControl.getLevelId());
			}
			
// 			if(buttonGreen->get()){
// 				wait(1);
// 				if(buttonBlue->get()){
// 					cbSeq.start();
// 				}else{
// 					safetySys.triggerEvent(properties.doAutoMoving);
// 					waitForLevel(properties.slAutoMoving.getLevelId());
// 					controlSys.setCircleInput(); // TODO right place??
// 				}
// 			}
// 			if(buttonBlue->get()){						// start CalibrateBlockSequence
// 				wait(1);
// 				if(buttonGreen->get()){
// 					cbSeq.start();
// 				}else{
// 					blueButtonCounter++;
// 				}
// 				if(blueButtonCounter >= 3){                    // doParking and stop the application
// 					blueButtonCounter = 0;
// 					ledBlue->set(false);
// 					safetySys.triggerEvent(properties.doParking);
// 					waitForLevel(properties.slParking.getLevelId());
// 					parkSeq.start();
// 					waitForLevel(properties.slParked.getLevelId());
// 					safetySys.triggerEvent(properties.doControlStop);
// 					waitForLevel(properties.slControlStopping.getLevelId());
// 				}
// 			}
		}
		else if(safetySys.getCurrentLevel() == properties.slAutoMoving){
			amSeq.start();
		}
		else if(safetySys.getCurrentLevel() == properties.slMouseControl){
			mouseSeq.start();
		}
		wait(0.1);
	}
	
	ledBlue->set(false);
	
	while(safetySys.getCurrentLevel() == properties.slEmergency){
		if(buttonGreen->get()){
			safetySys.triggerEvent(properties.doControlStart);
			break;                                                 // should restart the mainsequence, due to emergencyMonitor
		}
	}
}


