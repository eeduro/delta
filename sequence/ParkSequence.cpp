#include "ParkSequence.hpp"

using namespace eeduro::delta;

ParkSequence::ParkSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
    : Sequence(name, caller, true),
      controlSys(cs),
      safetyProp(sp),
      safetySys(ss),
      move("Move",this, cs) { }

int ParkSequence::action() {	
  move({0, 0, -0.06});
  controlSys.voltageSetPoint.setValue({0, 0, 0});	
  controlSys.voltageSwitch.switchToInput(1);
  safetySys.triggerEvent(safetyProp.parkingDone);
  return(0);
}
