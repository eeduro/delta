#include "CircleSequence.hpp"

using namespace eeduro::delta;

CircleSequence::CircleSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
    : Sequence(name, caller, true),
      controlSys(cs),
      safetySys(ss),
      safetyProp(sp),
      wait("Wait in circle sequence", this),
      move("Move in circle sequence", this, cs),
      moveMouseCondition(cs),
      mouseExceptionSequence("Mouse exception sequence", this,  ss, sp),
      moveMouseMonitor("Mouse move monitor", this, moveMouseCondition, SequenceProp::abort, &mouseExceptionSequence),
      blueButtonCondition(),
      blueButtonExceptionSequence("Blue button exception sequence in circle", this, cs, ss, sp),
      blueButtonMonitor("Blue button monitor", this, blueButtonCondition, SequenceProp::abort, &blueButtonExceptionSequence) { 
    addMonitor(&blueButtonMonitor);
    addMonitor(&moveMouseMonitor);
  }


int CircleSequence::action() {
  controlSys.pathPlanner.setStart(controlSys.directKin.getOut().getSignal().getValue());
  controlSys.setPathPlannerInput();
  move({circleRadius, 0, tcpReady_z});
  controlSys.circlePlanner.setInitPos({circleRadius, 0, tcpReady_z});
  controlSys.setCircleInput();

  while (Sequencer::running && safetySys.getCurrentLevel() == safetyProp.slAutoMoving) {
    wait(0.2);
  }
  return(0);
}

void CircleSequence::resetMousePos() {
  moveMouseCondition.reset();
}
