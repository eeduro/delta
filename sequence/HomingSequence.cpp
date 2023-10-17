#include "HomingSequence.hpp"

using namespace eeduro::delta;

HomingSequence::HomingSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
    : Sequence(name, caller, true),
      controlSys(cs),
      safetySys(ss),
      safetyProp(sp),
      move("Move", this, cs),
      wait("Wait", this) { }

int HomingSequence::action() {
  controlSys.voltageSetPoint.setValue({q012InitVoltage, q012InitVoltage, q012InitVoltage});	// choose fixed voltage
  controlSys.voltageSwitch.switchToInput(1);
  wait(2.5);
  controlSys.enc1.callInputFeature<>("setFqdPos", q012homingOffset);
  controlSys.enc2.callInputFeature<>("setFqdPos", q012homingOffset);
  controlSys.enc3.callInputFeature<>("setFqdPos", q012homingOffset);
  wait(0.1);
  controlSys.pathPlanner.setStart(controlSys.directKin.getOut().getSignal().getValue());
  controlSys.setPathPlannerInput();
  wait(0.1);
  controlSys.voltageSwitch.switchToInput(0);	// choose controller setpoint
  move({0, 0, tcpReady_z});
  safetySys.triggerEvent(safetyProp.homingDone);
  return(0);
}
