#include "MainSequence.hpp"

using namespace eeduro::delta;

MainSequence::MainSequence(std::string name, Sequencer& seq, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
    : Sequence(name, seq),
      safetyProp(sp),
      safetySys(ss),
      controlSys(cs),
      homingSeq("Homing sequence", this, cs, ss, sp),
      testingSeq("Testing sequence", this, cs, ss, sp),
      wait("Wait in main", this),
      emergencyCondition(ss, sp),
      emergencyExceptionSeq("Emergency exception sequence", this, cs), 
      emergencyMonitor("Emergency level monitor", this, emergencyCondition, eeros::sequencer::SequenceProp::restart, &emergencyExceptionSeq) {
    HAL& hal = HAL::instance();
    buttonGreen = hal.getLogicInput("buttonGreen", false);
    buttonBlue = hal.getLogicInput("buttonBlue", false);
    ledGreen = hal.getLogicOutput("ledGreen", false);
    ledBlue = hal.getLogicOutput("ledBlue", false);
    addMonitor(&emergencyMonitor);
  }
  
int MainSequence::action() {
  bool bluePrev;
  ledGreen->set(false);
  ledBlue->set(false);
  
  while(Sequencer::running) {
    bool broadSide;
    if(safetySys.getCurrentLevel() == safetyProp.slSystemOn) {
      safetySys.triggerEvent(safetyProp.doPoweringUp);
    }
    if(safetySys.getCurrentLevel() == safetyProp.slEmergency) {
      if(buttonBlue->get())
        safetySys.triggerEvent(safetyProp.doControlStart);
    }
    if(safetySys.getCurrentLevel() == safetyProp.slPoweringUp) {
      safetySys.triggerEvent(safetyProp.doHoming);
    }
    if(safetySys.getCurrentLevel() == safetyProp.slHoming) {
      homingSeq();
    }
    if(safetySys.getCurrentLevel() == safetyProp.slHomed) {
      safetySys.triggerEvent(safetyProp.doSystemReady);
    }
    if(safetySys.getCurrentLevel() == safetyProp.slSystemReady) {
      ledGreen->set(true);
      ledBlue->set(true);
      if(buttonGreen->get()) {
        broadSide = true;
        safetySys.triggerEvent(safetyProp.doTesting);     // go to testing
      }
      if(buttonBlue->get()) {
        broadSide = false;
        safetySys.triggerEvent(safetyProp.doTesting);     // go to testing
      }
    }
    else if(safetySys.getCurrentLevel() == safetyProp.slTesting) {
      ledGreen->set(false);
      ledBlue->set(false);
      testingSeq(broadSide);
    }
    wait(1);
  }
  return(0);
}


