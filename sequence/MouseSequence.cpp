#include "MouseSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "ExceptionSequence.hpp"

using namespace eeduro::delta;

MouseSequence::MouseSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp) 
    : Sequence(name, caller, true),
      controlSys(cs),
      safetySys(ss),
      safetyProp(sp),
      wait("Wait", this),
      mouseTimeoutSequence("Mouse timeOut exception", this, ss, sp),
      blueButtonCondition(),
      blueButtonExceptionSequence("Blue button exception in mouse", this, cs, ss, sp),
      blueButtonMonitor("Blue button monitor", this, blueButtonCondition, SequenceProp::abort, &blueButtonExceptionSequence) {
    setTimeoutTime(5.0);
    setTimeoutExceptionSequence(mouseTimeoutSequence);
    setTimeoutBehavior(SequenceProp::abort);
    addMonitor(&blueButtonMonitor);
  }

int MouseSequence::action() {
  controlSys.setMouseInput();
  auto pos = controlSys.circlePlanner.getOut().getSignal().getValue();
  controlSys.mouse.setInitPos(pos[0], pos[1], pos[2], 0);
  mousePosPrev = controlSys.redVect.getOut().getSignal().getValue();

  while (state == SequenceState::running && safetySys.getCurrentLevel() == safetyProp.slMouseControl) {
    auto pos = controlSys.redVect.getOut().getSignal().getValue();	
    if (pos != mousePosPrev) {
      resetTimeout();
      mousePosPrev = pos;
    }
    wait(0.1);
  }
  return(0);
}


