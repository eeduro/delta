#pragma once

#include <eeros/control/Blockio.hpp>
#include <eeros/math/Matrix.hpp>

#include "types.hpp"
#include "Kinematic.hpp"

using namespace eeros::math;
using namespace eeros;
using namespace eeros::control;
using namespace eeduro::delta;

namespace eeduro {
namespace delta {
  
class ReduceVector : public Blockio<1,1,Vector4, AxisVector> {
 public:
  ReduceVector() { }
  virtual void run() { 
    auto pos = in.getSignal().getValue().getSubMatrix<3, 1>(0, 0);
    AxisVector newPos;
    newPos[0] = pos[1];
    newPos[1] = pos[0];
    newPos[2] = pos[2];
    out.getSignal().setValue(newPos);
    out.getSignal().setTimestamp(in.getSignal().getTimestamp());
  }
};

}
}

