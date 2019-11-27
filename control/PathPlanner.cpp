#include "PathPlanner.hpp"

using namespace eeduro::delta;

PathPlanner::PathPlanner(AxisVector velMax, AxisVector accMax, double dt) : trajectoryGen(velMax, accMax, -accMax, dt) { }

Output<AxisVector>& PathPlanner::getPosOut() {
	return posOut;
}

Output<AxisVector>& PathPlanner::getVelOut() {
	return velOut;
}

Output<AxisVector>& PathPlanner::getAccOut() {
	return accOut;
}

void PathPlanner::gotoPoint(AxisVector p) {
	TrajectoryGenerator<AxisVector, 3>* t = static_cast<TrajectoryGenerator<AxisVector, 3>*>(&trajectoryGen);
	t->push(p);
	lastPoint = p;
}

AxisVector PathPlanner::getLastPoint() {
	return lastPoint;
}

bool PathPlanner::posReached() {
	return trajectoryGen.finished();
}

void PathPlanner::setInitPos(AxisVector initPos) {
	AxisVector z; z.zero();
	std::array<AxisVector, 3> r;
	r[0] = initPos;
	r[1] = z;
	r[2] = z;
	trajectoryGen.reset(r);
	lastPoint = initPos;
}

void PathPlanner::run() {
	std::array<AxisVector, 3> x;
	double t = System::getTime();
	x = trajectoryGen.get(dt);
	
	posOut.getSignal().setValue(x[0]);
	posOut.getSignal().setTimestamp(t);
	velOut.getSignal().setValue(x[1]);
	velOut.getSignal().setTimestamp(t);
	accOut.getSignal().setValue(x[2]);
	accOut.getSignal().setTimestamp(t);
}
