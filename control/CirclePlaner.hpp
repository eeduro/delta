#pragma once

#include <eeros/core/System.hpp>
#include <eeros/control/Block1o.hpp>
#include "types.hpp"

using namespace eeros;
using namespace eeros::control;

namespace eeduro {
namespace delta {
  
class CirclePlaner : public Block1o<AxisVector> {
 public:
  CirclePlaner(double radius, double speed);
  
  virtual void setInitPos(double x, double y, double z);
  virtual void setInitPos(AxisVector pos);
  virtual void run();

 protected:
  double R, v, start;
};

}
}
