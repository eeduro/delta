#include "AutoMoveSequence.hpp"

using namespace eeduro::delta;

AutoMoveSequence::AutoMoveSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp, Calibration& cal):
  Sequence(name, caller, true),
  sortSeq("Sort sequence", this, cs, cal),
  shuffSeq("Shuffle sequence", this, cs, cal),
  wait("Wait in auto move", this),
  moveMouseCondition(cs),
  mouseExceptionSeq("Mouse exception sequence", this,  ss, sp),
  moveMouseMonitor("Mouse move monitor", this, moveMouseCondition, SequenceProp::abort, &mouseExceptionSeq),
  controlSys(cs),
  safetySys(ss),
  safetyProp(sp),
  blueButtonCondition(),
  blueButtonExceptionSeq("Blue button exception sequence in auto move", this, cs, ss, sp),
  blueButtonMonitor("Blue button monitor", this, blueButtonCondition, SequenceProp::abort, &blueButtonExceptionSeq) { 
    addMonitor(&moveMouseMonitor);
    addMonitor(&blueButtonMonitor);
  }
  


int AutoMoveSequence::action() {
  moveMouseCondition.reset();
  controlSys.setPathPlannerInput();
  while(Sequencer::running) {
    auto res = sortSeq();
    moveMouseCondition.reset();
    wait(5);
    if (res == 0) shuffSeq();
    wait(5);
  }
  return(0);
}


