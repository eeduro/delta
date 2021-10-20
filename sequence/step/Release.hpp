#pragma once

#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include "../../control/DeltaControlSystem.hpp"
#include <unistd.h>

using namespace eeros::sequencer;

namespace eeduro {
namespace delta {
  
class Release : public Step {
 public:
  Release(std::string name, Sequence* caller, DeltaControlSystem& cs) : Step(name, caller), controlSys(cs) { }
  int action() {controlSys.emagVal.setValue(false); usleep(10000); return(0);}  // wait for the magnet to run dry
  
 private:
  DeltaControlSystem &controlSys;
};

}
}
