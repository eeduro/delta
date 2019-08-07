#include "Circle.hpp"

using namespace eeduro::delta;

Circle::Circle(double radius, double speed) : R(radius), v(speed) { }


Output<AxisVector>& Circle::getOut() {
	return pos;
}

void Circle::run() {
	AxisVector c;
	double t = System::getTime();
	
	c[0] = R*cos(v*t);
	c[1] = R*sin(v*t);
	c[2] = tcpReady_z;
	c[3] = tcpReady_phi;
	
	pos.getSignal().setValue(c);
	pos.getSignal().setTimestamp(t);
}
