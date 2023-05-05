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
  
class DirectKinematic : public Blockio<1,1,AxisVector> {
 public:
  DirectKinematic(Kinematic& kin);

  virtual void run();
  
 private:
  Kinematic& kinematic;
};

}
}

