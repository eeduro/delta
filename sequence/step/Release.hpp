#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>

#include "../../control/DeltaControlSystem.hpp"

using namespace eeros::sequencer;

namespace eeduro {
namespace delta {
  
class Release : public Step {
 public:
  Release(std::string name, Sequence* caller, DeltaControlSystem& cs) : Step(name, caller), controlSys(cs) { }
  int action() {controlSys.emagVal.setValue(false);}
  
 private:
  DeltaControlSystem &controlSys;
};

}
}
