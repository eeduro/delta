#include "Inertia.hpp"
#include "constants.hpp"

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
	Vector3 q012, xyz;
	
	q012 = jointPosIn.getSignal().getValue().getSubMatrix<3,1>(0, 0);
	xyz = tcpPosIn.getSignal().getValue().getSubMatrix<3,1>(0, 0);
	Vector3 a = accelerationIn.getSignal().getValue().getSubMatrix<3,1>(0, 0);
	double qdd = accelerationIn.getSignal().getValue()[3];
	
	if (jacobi.calculate(q012, xyz)) {
		Matrix<3,3> jacobian = jacobi.getJacobian();
		if (jacobian.isInvertible()) {
			Matrix<3,3> inverseJacobian = !jacobian;
			Matrix<3,3> inverseJacobianTransposed = inverseJacobian.transpose();

// 			a[0] = 0; // Regelung in x-Richtung ausschalten
// 			a[1] = 0; // Regelung in y-Richtung ausschalten
// 			a[2] = 0; // Regelung in z-Richtung ausschalten

			Matrix<3,3> M = tcpMass + inverseJacobianTransposed * motorInertia * inverseJacobian;
			Vector3 F = M * a;
			double tau = jred * qdd; // magnet axis

			forceOut.getSignal().setValue(Vector4(F[0], F[1], F[2], tau));
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