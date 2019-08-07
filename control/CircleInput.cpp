#include "CircleInput.hpp"

using namespace eeduro::delta;

CircleInput::CircleInput(double radius, double speed) : R(radius), v(speed) { }


Output<AxisVector>& CircleInput::getOut() {
	return pos;
}

void CircleInput::run() {
	AxisVector c;
	double t = System::getTime();
	
	c[0] = R*cos(v*t);
	c[1] = R*sin(v*t);
	c[2] = tcpReady_z;
	c[3] = tcpReady_phi;
	
	pos.getSignal().setValue(c);
	pos.getSignal().setTimestamp(t);
}

void CircleInput::setInitPos(double x, double y, double z, double r) {
	pos.getSignal().setValue(Matrix<4>{ x, y, z, r });
}

void CircleInput::setInitPos(Matrix<4> pos) {
	setInitPos(pos[0], pos[1], pos[2], pos[3]);
}
