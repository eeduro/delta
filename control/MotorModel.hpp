#pragma once

#include <eeros/control/Block1i1o.hpp>
#include "types.hpp"

using namespace eeros;
using namespace eeros::control;

namespace eeduro {
namespace delta {
  
class MotorModel : public Block {
 public:
  MotorModel(const AxisVector kM, const AxisVector RA);
  
  virtual control::Input<AxisVector>& getTorqueIn();
  virtual control::Input<AxisVector>& getSpeedIn();
  virtual control::Output<AxisVector>& getOut();
  
  virtual void run();
  
 protected:
  control::Input<AxisVector> torque;
  control::Input<AxisVector> speed;
  control::Output<AxisVector> voltage;
  
  AxisVector kM, RA;
};

}
}
