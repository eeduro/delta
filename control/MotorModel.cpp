#include "MotorModel.hpp"

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
  AxisVector u, M, w, gear;
  M = torque.getSignal().getValue();
  w = speed.getSignal().getValue();
  gear = {i1524, i1524, i1524, i0816};
  for(unsigned int i = 0; i < u.size(); i++) {
    u[i] = RA[i] * M[i] / ( kM[i] * gear[i]) + w[i] * kM[i] * gear[i];
  }
  voltage.getSignal().setValue(u);
  voltage.getSignal().setTimestamp(torque.getSignal().getTimestamp());
}
