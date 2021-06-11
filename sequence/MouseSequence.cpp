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
      grab("grab", this, cs),
      release("release", this, cs),
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
  auto pos = controlSys.pathPlanner.getPosOut().getSignal().getValue();
  controlSys.mouse.setInitPos(pos[0], pos[1], pos[2], 0);
  mousePosPrev = controlSys.mouse.getOut().getSignal().getValue();
  mouseButtonPrev = controlSys.mouse.getButtonOut().getSignal().getValue();

  while (Sequencer::running && safetySys.getCurrentLevel() == safetyProp.slMouseControl) {
    auto pos = controlSys.mouse.getOut().getSignal().getValue();	
    auto button = controlSys.mouse.getButtonOut().getSignal().getValue();
    if (pos != mousePosPrev || button != mouseButtonPrev) {
      resetTimeout();
      mousePosPrev = pos;
      mouseButtonPrev = button;
    }
    if (button[0] == 1) grab(); else release();
    wait(0.2);
  }
  return(0);
}


