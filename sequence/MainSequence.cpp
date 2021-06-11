#include "MainSequence.hpp"

using namespace eeduro::delta;

MainSequence::MainSequence(std::string name, Sequencer& seq, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp, Calibration& cal)
    : Sequence(name, seq),
      controlSys(cs),
      safetySys(ss),
      safetyProp(sp),
      homingSeq("Homing", this, cs, ss, sp),
      automoveSeq("AutoMove", this, cs, ss, sp, cal),
      parkSeq("Park", this, cs, ss, sp),
      calibrateSeq("Calibrate", this, cs, cal),
      mouseSeq("Mouse sequence", this, cs, ss, sp),
      wait("Wait", this),
      emergencyCondition(ss, sp),
      emergencyExceptionSeq("Emergency exception in main", this, cs, ss, sp), 
      emergencyMonitor("Emergency level monitor", this, emergencyCondition, eeros::sequencer::SequenceProp::restart, &emergencyExceptionSeq) {
    HAL& hal = HAL::instance();
    buttonGreen = hal.getLogicInput("buttonGreen", false);
    buttonBlue = hal.getLogicInput("buttonBlue", false);
    ledBlue = hal.getLogicOutput("ledBlue", false);
    addMonitor(&emergencyMonitor);
  }
  
int MainSequence::action() {
  bool bluePrev;
  ledBlue->set(false);
  
  while(state == SequenceState::running) {
    if(safetySys.getCurrentLevel() == safetyProp.slSystemOn) {
      safetySys.triggerEvent(safetyProp.doPoweringUp);
    }
    if(safetySys.getCurrentLevel() == safetyProp.slPoweringUp) {
      safetySys.triggerEvent(safetyProp.doHoming);
    }
    if(safetySys.getCurrentLevel() == safetyProp.slHoming) {
      homingSeq();
    }
    if(safetySys.getCurrentLevel() == safetyProp.slHomed) {
      ledBlue->set(true);
      bluePrev = buttonBlue->get();
      safetySys.triggerEvent(safetyProp.doSystemReady);
    }
    if(safetySys.getCurrentLevel() == safetyProp.slSystemReady) {
      
      if(buttonGreen->get()) {
        blueButtonCounter = 0;
        if(buttonBlue->get()) calibrateSeq();
				else safetySys.triggerEvent(safetyProp.doAutoMoving);     // go to auto moving
      }
      auto blue = buttonBlue->get();
      if(blue && blue != bluePrev) {
        blueButtonCounter++;
        if(blueButtonCounter >= 3) {                    // doParking and stop the application
          ledBlue->set(false);
          safetySys.triggerEvent(safetyProp.doParking);
        }
      }
      bluePrev = blue;
    }
    else if(safetySys.getCurrentLevel() == safetyProp.slAutoMoving) {
      automoveSeq.resetMousePos();
      automoveSeq();
    }
    else if(safetySys.getCurrentLevel() == safetyProp.slMouseControl) {
      mouseSeq();
    }
    else if(safetySys.getCurrentLevel() == safetyProp.slParking) {
      parkSeq();
    }
    else if(safetySys.getCurrentLevel() == safetyProp.slParked) {
      safetySys.triggerEvent(safetyProp.doControlStop);
    }
    wait(0.2);
  }
  return(0);
}


