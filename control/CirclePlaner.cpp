#include "CirclePlaner.hpp"

using namespace eeduro::delta;

CirclePlaner::CirclePlaner(double radius, double speed) : R(radius), v(speed) { }

void CirclePlaner::run() {
  AxisVector c;
  double t = System::getTime();
  
  c[0] = R * cos(v * (t - start));
  c[1] = R * sin(v * (t - start));
  c[2] = tcpReady_z;
  
  out.getSignal().setValue(c);
  out.getSignal().setTimestamp(t);
}

void CirclePlaner::setInitPos(double x, double y, double z) {
  setInitPos(AxisVector{x, y, z});
}

void CirclePlaner::setInitPos(AxisVector pos) {
  out.getSignal().setValue(pos);
  start = System::getTime();
}
