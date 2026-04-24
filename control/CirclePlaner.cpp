#include "CirclePlaner.hpp"

using namespace eeduro::delta;

CirclePlaner::CirclePlaner(double radius, double speed) : R(radius), v(speed) { }

void CirclePlaner::run() {
  AxisVector c;
  double t = System::getTime();
  
  c[0] = R * cos(v * (t - start));
  c[1] = R * sin(v * (t - start));
  c[2] = tcpReady_z;
  
  this->getOut().getSignal().setValue(c);
  this->getOut().getSignal().setTimestamp(t);
}

void CirclePlaner::setInitPos(double x, double y, double z) {
  setInitPos(AxisVector{x, y, z});
}

void CirclePlaner::setInitPos(AxisVector pos) {
  this->getOut().getSignal().setValue(pos);
  start = System::getTime();
}
