#pragma once

#include <vector>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "step/Move.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
namespace delta {

class EmergencyExceptionSequence : public Sequence{
 public:
  EmergencyExceptionSequence(std::string name, Sequence* caller, DeltaControlSystem& cs) 
      : Sequence(name, caller, true),
        cs(cs) { }
    
  int action(){
    cs.voltageSetPoint.setValue({0, 0, 0});
    cs.voltageSwitch.switchToInput(1);
    HAL::instance().getLogicOutput("ledBlue", false)->set(true);
    HAL::instance().getLogicOutput("ledRed", false)->set(false);
    HAL::instance().getLogicOutput("ledGreen", false)->set(false);
    return(0);
  }

 private:
  DeltaControlSystem& cs;
};

}
}
