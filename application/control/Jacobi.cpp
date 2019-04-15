#include "Jacobi.hpp"

using namespace eeduro::delta;

Jacobi::Jacobi(Jacobian& j) : jacobi(j) { }

void Jacobi::run() {
	Vector3 q012, xyz, f, t012;
	AxisVector t0123;
	
	q012 = jointPosIn.getSignal().getValue().getSubMatrix<3, 1>(0, 0);
	xyz = tcpPosIn.getSignal().getValue().getSubMatrix<3, 1>(0, 0);
	f = forceIn.getSignal().getValue().getSubMatrix<3, 1>(0, 0);
	
	if(jacobi.calculate(q012, xyz)) {
		t012 = jacobi.getDrivetorque(f);
	}
	else {
		t012 = 0;
	}
	
	t0123[0] = t012[0];
	t0123[1] = t012[1];
	t0123[2] = t012[2];
	t0123[3] = forceIn.getSignal().getValue()[3];
	
	torqueOut.getSignal().setValue(t0123);
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