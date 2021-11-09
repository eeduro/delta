#include "MainSequence.hpp"

using namespace eeduro::delta;

MainSequence::MainSequence(std::string name, Sequencer& seq, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
    : Sequence(name, seq),
      safetyProp(sp),
      safetySys(ss),
      controlSys(cs),
      homingSeq("Homing sequence", this, cs, ss, sp),
      testingSeq("Testing sequence", this, cs, ss, sp),
      parkSeq("Park sequence", this, cs, ss, sp),
      mouseSeq("Mouse sequence", this, cs, ss, sp),
      wait("Wait in main", this),
      emergencyCondition(ss, sp),
      emergencyExceptionSeq("Emergency exception sequence", this, cs, ss, sp), 
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
  
  while(Sequencer::running) {
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
        safetySys.triggerEvent(safetyProp.doMoving);     // go to moving
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
    else if(safetySys.getCurrentLevel() == safetyProp.slMoving) {
      testingSeq.resetMousePos();
      testingSeq();
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


