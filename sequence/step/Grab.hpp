#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>

#include "../../control/DeltaControlSystem.hpp"

using namespace eeros::sequencer;

namespace eeduro {
namespace delta {
  
class Grab : public Step {
 public:
  Grab(std::string name, Sequence* caller, DeltaControlSystem& cs) : Step(name, caller), controlSys(cs) { }
  int action() {controlSys.emagVal.setValue(true);}
  
 private:
  DeltaControlSystem &controlSys; 
};

}
}
