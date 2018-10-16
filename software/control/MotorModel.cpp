#include "MotorModel.hpp"

using namespace eeros::control;
using namespace eeduro::delta;

MotorModel::MotorModel(const AxisVector kM, const AxisVector RA) : kM(kM), RA(RA) { }

Input<AxisVector>& MotorModel::getSpeedIn() {
	return speed;
}

Input<AxisVector>& MotorModel::getTorqueIn() {
	return torque;
}

Output<AxisVector>& MotorModel::getOut() {
	return voltage;
}

void MotorModel::run() {
	AxisVector u, M, w;
	M = torque.getSignal().getValue();
	w = speed.getSignal().getValue();
	for(unsigned int i = 0; i < u.size(); i++) {
		u[i] = RA[i] * M[i] / kM[i] + w[i] * kM[i];
	}
	voltage.getSignal().setValue(u);
	voltage.getSignal().setTimestamp(torque.getSignal().getTimestamp());
}
