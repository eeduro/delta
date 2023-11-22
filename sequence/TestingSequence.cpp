#include "TestingSequence.hpp"


using namespace eeduro::delta;

Check::Check(std::string name, Sequence* caller, DeltaControlSystem& cs) 
    : Sequence(name, caller, true),
      wait("Wait in check sequence", this),
      move("Move in check sequence", this, cs) { 
  ledGreen = HAL::instance().getLogicOutput("ledGreen", false);
}

int Check::operator() (AxisVector position) {this->position = position; return start();}

int Check::action() {
  log.info() << "check at pos = " << position;
  move(position);
  position[2] -= 0.01;
  move(position);
  log.info() << "test ready";
  ledGreen->set(true);
  wait(0.3);
  ledGreen->set(false);
  log.info() << "test done";
  position[2] += 0.01;
  move(position);

  return 0;
}

TestingSequence::TestingSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
    : Sequence(name, caller, true), cs(cs), ss(ss), sp(sp),
      wait("Wait in testing sequence", this),
      move("Move in testing sequence", this, cs),
      check("Check", this, cs) { 
  ledRed = HAL::instance().getLogicOutput("ledRed", false);
  ledGreen = HAL::instance().getLogicOutput("ledGreen", false);
}

int TestingSequence::operator() (bool broadSide) {this->broadSide = broadSide; return start();}

int TestingSequence::action() {
  cs.pathPlanner.setStart(cs.directKin.getOut().getSignal().getValue());
  ledGreen->set(true);
  ledRed->set(true);
  if (broadSide) wait(0.3); else wait(0.1);
  ledGreen->set(false);

  if (broadSide) {
    AxisVector pos = {tcpReady_x, tcpReady_y, tcpReady_z - 0.006}; // zero position is at U18/U15
    pos[0] = tcpReady_x + 3 * 0.014 + 0.002; // MIO
    for (int k = 1; k >= -3; k--) {
      pos[1] = k * 0.0074;
      check(pos);
    }
    for (int i = 2; i >= -2; i--) { // flink GPIO
      pos[0] = tcpReady_x + i * 0.014;
      for (int k = 3; k >= -3; k--) {
        pos[1] = k * 0.0074;
        if (i == -2 && k == 3) pos[1] += 0.001; // kinematic does is not precise at outer range
        if (i == -2 && k == 2) pos[1] += 0.0009;
        if (i == -1 && k == 3) pos[1] += 0.0007;
        if (i == 0 && k == 3) pos[1] += 0.0005;
        check(pos);
      }
    }
  } else {
    AxisVector pos = {tcpReady_x, tcpReady_y, tcpReady_z - 0.006}; // zero position is at G18/F20
    for (int i = 4; i >= -3; i--) {
      pos[0] = tcpReady_x + i * 0.014;
      if (i == 4) pos[0] += 0.0006;
      if (i <= -1) pos[0] -= 0.0006;
      for (int k = 2; k >= -2; k--) {
        pos[1] = k * 0.0074;
        if (i == -3 && k >= 1) pos[1] += 0.001;
        if (i == -2 && k == 2) pos[1] += 0.001;
        check(pos);
      }
    }
  }
  
  ledRed->set(false);
  move({tcpReady_x, tcpReady_y, tcpReady_z});
  ss.triggerEvent(sp.stopMoving);
  return(0);
}

