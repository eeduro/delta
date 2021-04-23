#include "Inertia.hpp"

using namespace eeduro::delta;

Inertia::Inertia(Jacobian &jacobi) : jacobi(jacobi) {
  tcpMass = 0;
  for (int i = 0; i < 3; i++)
    tcpMass(i,i) = mtcp;
      
  motorInertia = 0;
  for (int i = 0; i < 3; i++)
    motorInertia(i,i) = jred;
}

void Inertia::run() {
  Vector3 a = accelerationIn.getSignal().getValue();
  
  if (jacobi.calculate(jointPosIn.getSignal().getValue(), tcpPosIn.getSignal().getValue())) {
    Matrix<3,3> jacobian = jacobi.getJacobian();
    if (jacobian.isInvertible()) {
      Matrix<3,3> inverseJacobian = !jacobian;
      Matrix<3,3> inverseJacobianTransposed = inverseJacobian.transpose();

      Matrix<3,3> M = tcpMass + inverseJacobianTransposed * motorInertia * inverseJacobian;
      Vector3 F = M * a;

      forceOut.getSignal().setValue(AxisVector(F[0], F[1], F[2]));
      forceOut.getSignal().setTimestamp(accelerationIn.getSignal().getTimestamp());
      
      return;
    }
  }
  
  forceOut.getSignal().setValue(0);
  forceOut.getSignal().setTimestamp(accelerationIn.getSignal().getTimestamp());
}

Input<AxisVector>& Inertia::getAccelerationInput() {
  return accelerationIn;
}

Input<AxisVector>& Inertia::getTcpPosInput() {
  return tcpPosIn;
}

Input<AxisVector>& Inertia::getJointPosInput() {
  return jointPosIn;
}

Output<AxisVector>& Inertia::getOut() {
  return forceOut;
}
