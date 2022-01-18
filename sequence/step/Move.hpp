#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/logger/Logger.hpp>

#include "../../control/DeltaControlSystem.hpp"

using namespace eeros::sequencer;
using namespace eeros::logger;

namespace eeduro {
namespace delta {
  
class Move : public Step {
 public:
  Move(std::string name, Sequence* caller, DeltaControlSystem& cs) : Step(name, caller), cs(cs) {
    this->position = {0,0,0};
  }
  int operator() (AxisVector position) {this->position = position; return start();}
  int action() {
    cs.pathPlanner.move(position); 
    return 0;
  }
  bool checkExitCondition() {return cs.pathPlanner.endReached();}
  
 private:
  DeltaControlSystem &cs;
  AxisVector position;
};

}
}
