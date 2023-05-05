#pragma once

#include <eeros/control/Blockio.hpp>
#include "constants.hpp"
#include "types.hpp"

using namespace eeros::control;

namespace eeduro {
namespace delta {
  
class MouseScale : public Blockio<1,1,AxisVector> {
 public:
  MouseScale() {}

  virtual void run() {
    auto pos = this->getIn().getSignal().getValue();
    AxisVector newPos;
    newPos[0] = pos[1];
    newPos[1] = pos[0];
    newPos[2] = pos[2];
    newPos[3] = pos[3];
    this->getOut().getSignal().setValue(newPos);
    this->getOut().getSignal().setTimestamp(this->getIn().getSignal().getTimestamp());
  }
  
 private:
  Vector3 offset; 		  /**< Offset TCP for example, the length of the magnet in the z-axis*/
};

}
}
