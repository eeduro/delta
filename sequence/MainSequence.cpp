#include "MainSequence.hpp"

using namespace eeduro::delta;

MainSequence::MainSequence(std::string name, Sequencer& seq, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
    : Sequence(name, seq),
      cs(cs),
      ss(ss),
      sp(sp),
      homingSeq("Homing sequence", this, cs, ss, sp),
      testingSeq("Testing sequence", this, cs, ss, sp),
      wait("Wait in main", this),
      emergencyExceptionSeq("Emergency exception sequence", this, cs),
      emergencyCondition(ss, sp),
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
    if(ss.getCurrentLevel() == sp.slSystemOn) {
      ss.triggerEvent(sp.doPoweringUp);
    }
    if(ss.getCurrentLevel() == sp.slEmergency) {
      if(buttonBlue->get())
        ss.triggerEvent(sp.doControlStart);
    }
    if(ss.getCurrentLevel() == sp.slPoweringUp) {
      ss.triggerEvent(sp.doHoming);
    }
    if(ss.getCurrentLevel() == sp.slHoming) {
      homingSeq();
    }
    if(ss.getCurrentLevel() == sp.slHomed) {
      ss.triggerEvent(sp.doSystemReady);
    }
    if(ss.getCurrentLevel() == sp.slSystemReady) {
      ledGreen->set(true);
      ledBlue->set(true);
      if(buttonGreen->get()) {
        broadSide = false;
        ss.triggerEvent(sp.doTesting);     // go to testing
      }
      if(buttonBlue->get()) {
        broadSide = true;
        ss.triggerEvent(sp.doTesting);     // go to testing
      }
    }
    else if(ss.getCurrentLevel() == sp.slTesting) {
      ledGreen->set(false);
      ledBlue->set(false);
      testingSeq(broadSide);
    }
    wait(1);
  }
  return(0);
}


