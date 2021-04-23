#include "Jacobi.hpp"

using namespace eeduro::delta;

Jacobi::Jacobi(Jacobian& j) : jacobi(j) { }

void Jacobi::run() {
  AxisVector t012;
  
  if(jacobi.calculate(jointPosIn.getSignal().getValue(), tcpPosIn.getSignal().getValue())) {
    t012 = jacobi.getDrivetorque(forceIn.getSignal().getValue());
  }
  else {
    t012 = 0;
  }
  
  torqueOut.getSignal().setValue(t012);
  torqueOut.getSignal().setTimestamp(forceIn.getSignal().getTimestamp());
}

Input<AxisVector>& Jacobi::getForceInput() {
  return forceIn;
}

Input<AxisVector>& Jacobi::getTcpPosInput() {
  return tcpPosIn;
}

Input<AxisVector>& Jacobi::getJointPosInput() {
  return jointPosIn;
}

Output<AxisVector>& Jacobi::getOut() {
  return torqueOut;
}
