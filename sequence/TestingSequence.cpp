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
}

int TestingSequence::operator() (bool broadSide) {this->broadSide = broadSide; return start();}

int TestingSequence::action() {
  char ch;
  cs.pathPlanner.setStart(cs.directKin.getOut().getSignal().getValue());
  cs.setPathPlannerInput();
  ledRed->set(true);

  if (broadSide) {
    AxisVector pos = {0, 0, tcpReady_z}; // zero position is at U18/U15
    pos[0] = 3 * 0.014 + 0.002; // MIO
    for (int k = 1; k >= -3; k--) {
      pos[1] = k * 0.0074;
      check(pos);
    }
    for (int i = 2; i >= -2; i--) {
      pos[0] = i * 0.014;
      for (int k = 3; k >= -3; k--) {
        pos[1] = k * 0.0074;
        check(pos);
      }
    }
    move({0, 0, tcpReady_z});
  } else {
    AxisVector pos = {0, 0, tcpReady_z}; // zero position is at G18/F20
    for (int i = 4; i >= -3; i--) {
      pos[0] = i * 0.014;
      for (int k = 2; k >= -2; k--) {
        pos[1] = k * 0.0074;
        check(pos);
      }
    }
    move({0, 0, tcpReady_z});    
  }
  
  ledRed->set(false);
  ss.triggerEvent(sp.stopMoving);
  return(0);
}

