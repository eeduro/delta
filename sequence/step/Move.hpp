#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include "../../control/DeltaControlSystem.hpp"

using namespace eeros::sequencer;

namespace eeduro {
namespace delta {
  
class Move : public Step {
 public:
  Move(std::string name, Sequence* caller, DeltaControlSystem& cs) : Step(name, caller), controlSys(cs) {
    this->position = {0,0,0,0};
  }
  int operator() (AxisVector position) {this->position = position; return start();}
  int action() {controlSys.pathPlanner.move(position); return 0;}
  bool checkExitCondition() {
    Vector4 diff = controlSys.directKin.getOut().getSignal().getValue() - position;
    return abs(diff[0]) < limit && abs(diff[1]) < limit && abs(diff[2]) < limit && abs(diff[3]) < 10*limit;
  }
  
 private:
  DeltaControlSystem &controlSys;
  AxisVector position;
  const double limit = 0.0002;
};

}
}
