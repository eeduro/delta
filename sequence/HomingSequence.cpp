#include "HomingSequence.hpp"

using namespace eeduro::delta;

HomingSequence::HomingSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
    : Sequence(name, caller, true),
      controlSys(cs),
      safetySys(ss),
      safetyProp(sp),
      move("Move in homing", this, cs),
      wait("Wait in homing", this) {}

int HomingSequence::action() {
  AxisVector torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit };
  controlSys.torqueLimitation.setLimit(-torqueLimit, torqueLimit);
  AxisVector forceLimit{ 100, 100, 100 };
  controlSys.forceLimitation.setLimit(-forceLimit, forceLimit);
  AxisVector accLimit{ 100, 100, 100 };
  controlSys.accLimitation.setLimit(-accLimit, accLimit);
  controlSys.voltageSetPoint.setValue({q012InitVoltage, q012InitVoltage, q012InitVoltage});	// choose fixed voltage
  controlSys.voltageSwitch.switchToInput(1);
  wait(2.5);
  controlSys.enc1.callInputFeature<>("setFqdPos", q012homingOffset);
  controlSys.enc2.callInputFeature<>("setFqdPos", q012homingOffset);
  controlSys.enc3.callInputFeature<>("setFqdPos", q012homingOffset);
  wait(0.1);
  controlSys.pathPlanner.setStart(controlSys.directKin.getOut().getSignal().getValue());
  wait(0.1);
  controlSys.voltageSwitch.switchToInput(0);	// choose controller setpoint
  move({tcpReady_x, tcpReady_y, tcpReady_z});
  safetySys.triggerEvent(safetyProp.homingDone);
  return(0);
}
