#include "ParkSequence.hpp"

using namespace eeduro::delta;

ParkSequence::ParkSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
    : Sequence(name, caller, true),
      controlSys(cs),
      safetyProp(sp),
      safetySys(ss),
      move("Move",this, cs) { }

int ParkSequence::action() {
  controlSys.setPathPlannerInput();
  move({0, 0, -0.045, 0});
  controlSys.voltageSetPoint.setValue({0, 0, 0, 0});	
  controlSys.voltageSwitch.switchToInput(1);
  safetySys.triggerEvent(safetyProp.parkingDone);
  return(0);
}
