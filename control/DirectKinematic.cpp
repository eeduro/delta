#include "DirectKinematic.hpp"

DirectKinematic::DirectKinematic(Kinematic& kin) : 
	kinematic(kin) { }

void DirectKinematic::run() {
	AxisVector q012, xyz;

	q012 = in.getSignal().getValue().getSubMatrix<3, 1>(0, 0);

	if (!kinematic.forward(in.getSignal().getValue(), xyz)) 
		eeros::Fault("Calculation of forward kinematic failed!");

	out.getSignal().setValue(xyz);
	out.getSignal().setTimestamp(in.getSignal().getTimestamp());
}
