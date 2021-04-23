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
  SafetyLevel slParking;
  SafetyLevel slParked;
  SafetyLevel slAutoMoving;
  SafetyLevel slMouseControl;

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
  SafetyEvent doParking;
  SafetyEvent parkingDone;
  SafetyEvent doSystemReady;
  SafetyEvent doAutoMoving;
  SafetyEvent doMouseControl;
  SafetyEvent stopMoving;

 private:
  DeltaControlSystem& controlSys;
  
  /*
   * critical outputs
   */
  hal::Output<bool>* greenLed;
  hal::Output<bool>* errorLed;
  
  /* 
   * critical inputs
   */
  hal::Input<bool>* emergencyStop;
};

}
}
