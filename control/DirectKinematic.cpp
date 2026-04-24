#include "DirectKinematic.hpp"

DirectKinematic::DirectKinematic(Kinematic& kin) : 
  kinematic(kin) { }

void DirectKinematic::run() {
  AxisVector q012, xyz;

  q012 = this->getIn().getSignal().getValue().getSubMatrix<3, 1>(0, 0);

  if (!kinematic.forward(this->getIn().getSignal().getValue(), xyz)) 
    eeros::Fault("Calculation of forward kinematic failed!");

  this->getOut().getSignal().setValue(xyz);
  this->getOut().getSignal().setTimestamp(this->getIn().getSignal().getTimestamp());
}
