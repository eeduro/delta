#include "MoveBlockSequence.hpp"

using namespace eeduro::delta;

MoveBlockSequence::MoveBlockSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, Calibration& cal)
    : Sequence(name, caller, true),
      controlSys(cs),
      move("Move in block", this, cs),
      grab("Grab", this, cs),
      calibration(cal),
      release("Release", this, cs) { }

int MoveBlockSequence::operator()(int block, int from, int to) {
  this->block = block;
  this-> from = from;
  this-> to = to;
  return start();
}

int MoveBlockSequence::action() {
  log.warn() << "move block " << block << " from position " << from << " to " << to;

  auto p = controlSys.pathPlanner.getPosOut().getSignal().getValue();
  p[2] = calibration.transportation_height;
  move(p);
  
  p[0] = calibration.position[from].x;
  p[1] = calibration.position[from].y;
  if (p[3] > 1) p[3] = calibration.position[from].r;
  else p[3] = calibration.position[from].r + pi / 2;
  move(p);
  
  p[2] = calibration.position[from].zblockmax[block];
  move(p);
  
  grab();

  p[2] = calibration.transportation_height;
  move(p);

  p[0] = calibration.position[to].x;
  p[1] = calibration.position[to].y;
  if (p[3] > 1) p[3] = calibration.position[to].r;
  else p[3] = calibration.position[to].r + pi / 2.0;
  move(p);

  p[2] = calibration.position[to].zblockmax[block];
  move(p);
  
  release();
  
  p[2] = calibration.transportation_height;
  move(p);
  return(0);
}
