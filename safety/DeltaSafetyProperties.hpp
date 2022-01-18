#pragma once

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include <eeros/safety/InputAction.hpp>
#include <eeros/safety/OutputAction.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>

#include "../control/DeltaControlSystem.hpp"

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::safety;
using namespace eeros::sequencer;

namespace eeduro {
namespace delta {
  
class DeltaSafetyProperties : public SafetyProperties {
 public:
  DeltaSafetyProperties(DeltaControlSystem& controlSys);
  
  /*
   * Name all levels
   */
  SafetyLevel slOff;
  SafetyLevel slEmergency;
  SafetyLevel slControlStopping;
  SafetyLevel slControlStarting;
  SafetyLevel slSystemOn;
  SafetyLevel slPoweringUp;
  SafetyLevel slHoming;
  SafetyLevel slHomed;
  SafetyLevel slSystemReady;
  SafetyLevel slTesting;

  /*
   * Define all possible events
   */
  SafetyEvent doEmergency;
  SafetyEvent doControlStart;
  SafetyEvent doControlStop;
  SafetyEvent controlStoppingDone;
  SafetyEvent controlStartingDone;
  SafetyEvent doPoweringUp;
  SafetyEvent doHoming;
  SafetyEvent homingDone;
  SafetyEvent doSystemReady;
  SafetyEvent doTesting;
  SafetyEvent stopMoving;

 private:
  DeltaControlSystem& cs;
  // critical inputs
  hal::Input<bool>* buttonRed;
};

}
}
