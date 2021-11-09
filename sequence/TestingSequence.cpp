#include "TestingSequence.hpp"


using namespace eeduro::delta;

TestingSequence::TestingSequence (std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
    : Sequence(name, caller, true),
      uart("/dev/ttyS0"),
      cs(cs),
      ss(ss),
      sp(sp),
      wait("Wait in testing sequence", this),
      move("Move in testing sequence", this, cs),
      moveMouseCondition(cs),
      mouseExceptionSequence("Mouse exception sequence", this,  ss, sp),
      moveMouseMonitor("Mouse move monitor", this, moveMouseCondition, SequenceProp::abort, &mouseExceptionSequence),
      blueButtonCondition(),
      blueButtonExceptionSequence("Blue button exception sequence in testing", this, cs, ss, sp),
      blueButtonMonitor("Blue button monitor", this, blueButtonCondition, SequenceProp::abort, &blueButtonExceptionSequence) { 
    addMonitor(&blueButtonMonitor);
    addMonitor(&moveMouseMonitor);
  }


int TestingSequence::action() {
  char ch;
  cs.pathPlanner.setStart(cs.directKin.getOut().getSignal().getValue());
  cs.setPathPlannerInput();
  move({-0.02, 0, tcpReady_z});
  wait(3);
  move({-0.02, 0, tcpReady_z - 0.025});
  log.info() << "test ready";

  log.info() << "test ok";
//   move({0, 0, tcpReady_z + 0.01});
//   wait(3);
//   res = write(fd, "X", 1);
//   move({0.05, 0, tcpReady_z + 0.01});
//   wait(10);
  move({-0.02, 0, tcpReady_z});
  move({0, 0, tcpReady_z});

  while (Sequencer::running && ss.getCurrentLevel() == sp.slMoving) {
    wait(5);
    log.warn() << uart.getStr();
  }
  return(0);
}

void TestingSequence::resetMousePos() {
  moveMouseCondition.reset();
}
