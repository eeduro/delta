#include <eeros/sequencer/Sequencer.hpp>
#include "AutoMoveSequence.hpp"

using namespace eeduro::delta;

AutoMoveSequence::AutoMoveSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp, Calibration& cal):
  Sequence(name, caller, true),
  sortSeq("Sort", this, cs, cal),
  shuffSeq("Shuffle", this, cs, cal),
  wait("Wait", this),
  moveMouseCondition(cs),
  mouseExceptionSeq("Mouse exception in AutoMove", this,  ss, sp),
  moveMouseMonitor("Mouse move monitor", this, moveMouseCondition, SequenceProp::abort, &mouseExceptionSeq),
  controlSys(cs),
  safetySys(ss),
  safetyProp(sp),
  blueButtonCondition(),
  blueButtonExceptionSeq("Blue button exception in AutoMove", this, cs, ss, sp),
  blueButtonMonitor("Blue button monitor", this, blueButtonCondition, SequenceProp::abort, &blueButtonExceptionSeq) { 
    addMonitor(&moveMouseMonitor);
    addMonitor(&blueButtonMonitor);
  }
  


int AutoMoveSequence::action() {
  controlSys.setPathPlannerInput();
  while(state == SequenceState::running && safetySys.getCurrentLevel() == safetyProp.slAutoMoving) {
    auto res = sortSeq();
//     moveMouseCondition.reset();
    wait(5);
    if (res == 0) shuffSeq();
    wait(5);
  }
  return(0);
}

void AutoMoveSequence::resetConditions() {
  moveMouseCondition.reset();
  blueButtonCondition.reset();
}

