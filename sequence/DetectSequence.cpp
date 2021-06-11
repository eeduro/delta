#include "DetectSequence.hpp"

using namespace eeduro::delta;

DetectSequence::DetectSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, Calibration& cal): 
  Sequence(name, caller, true),
  controlSys(cs),
  calibration(cal),
  move("Move", this, cs),
  wait("Wait", this) { }

int DetectSequence::operator()(int pos) {
  this-> position = pos;
  return start();
}

int DetectSequence::action() {
  auto p = controlSys.pathPlanner.getPosOut().getSignal().getValue();
  p[3] = 0;
  double last_z = p[2];
  p[2] = calibration.position[position].level12 + 0.005;
  move(p);

  // switch to constant force
  eeros::math::Vector<4> limit{ 100, 100, 100, 100 };
  controlSys.accLimitation.setLimit({ -100, -100, 0, -100 }, limit);
  controlSys.forceSetPoint.setValue({0, 0, -0.7, 0});
  p[2] = calibration.position[position].level30 - 0.002;
  move(p);
  wait(0.5);	// wait for final position to be reached
  
  double z = controlSys.directKin.getOut().getSignal().getValue()[2];
  p[2] = last_z;
  controlSys.accLimitation.setLimit(-limit, limit);
  controlSys.forceSetPoint.setValue({0, 0, 0, 0});
  move(p);

  int block = calibration.getBlock(position, z);
  log.warn() << "[DETECT] pos " << position << ": z = " << z << " -> block = " << block;
  
  return block;
}

