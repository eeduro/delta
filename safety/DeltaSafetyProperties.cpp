#include "DeltaSafetyProperties.hpp"

using namespace eeduro::delta;

DeltaSafetyProperties::DeltaSafetyProperties(DeltaControlSystem& cs) 
    : cs(cs),

      doEmergency("Do emergency"),
      doControlStart("Do control start"),
      doControlStop("Do control stop"),
      controlStoppingDone("Control stop done"),
      controlStartingDone("Control start done"),
      doPoweringUp("Do power up"),
      doHoming("Do homing"),
      homingDone("Homing done"),
      doSystemReady("Do system ready"),
      doTesting("Do testing"),
      stopMoving("Stop moving"),
      
      slOff("Off"),
      slEmergency("Emergency"),
      slControlStopping("Stop control system"),
      slControlStarting("Start control system"),
      slSystemOn("System on"),
      slPoweringUp("Powering up"),
      slHoming("Homing"),
      slHomed("Homed"),
      slSystemReady("System ready"),
      slTesting("Testing") {
      
  HAL& hal = HAL::instance();
  buttonRed = hal.getLogicInput("buttonRed", false);
  criticalInputs = { buttonRed };	  
  
  addLevel(slOff);
  addLevel(slEmergency);
  addLevel(slControlStopping);
  addLevel(slControlStarting);
  addLevel(slSystemOn);
  addLevel(slPoweringUp);
  addLevel(slHoming);
  addLevel(slHomed);
  addLevel(slSystemReady);
  addLevel(slTesting);
  
  slControlStarting.addEvent(controlStartingDone, slSystemOn, kPrivateEvent);
  slSystemOn.addEvent(doPoweringUp, slPoweringUp, kPublicEvent);
  slPoweringUp.addEvent(doHoming, slHoming, kPublicEvent);
  slHoming.addEvent(homingDone, slHomed, kPublicEvent);
  slHomed.addEvent(doSystemReady, slSystemReady, kPublicEvent);
  slSystemReady.addEvent(doTesting, slTesting, kPublicEvent);
  slEmergency.addEvent(doControlStart, slControlStarting, kPublicEvent);
  slEmergency.addEvent(stopMoving, slEmergency, kPublicEvent);
  slTesting.addEvent(stopMoving, slSystemReady, kPublicEvent);
  slControlStopping.addEvent(controlStoppingDone, slOff, kPublicEvent);
  
  addEventToLevelAndAbove(slSystemOn, doEmergency, slEmergency, kPublicEvent);
  
  slOff.setInputActions({ignore(buttonRed)});
  slEmergency.setInputActions({ignore(buttonRed)});
  slControlStopping.setInputActions({ignore(buttonRed)});
  slControlStarting.setInputActions({ignore(buttonRed)});
  slSystemOn.setInputActions({ignore(buttonRed)});
  slPoweringUp.setInputActions({ignore(buttonRed)});
  slHoming.setInputActions({ignore(buttonRed)});
  slHomed.setInputActions({ignore(buttonRed)});
  slSystemReady.setInputActions({ignore(buttonRed)});
  slTesting.setInputActions({check(buttonRed, false, doEmergency)});

  slOff.setLevelAction([&](SafetyContext*privateContext){
    Executor::stop();
  });

  slControlStopping.setLevelAction([&](SafetyContext*privateContext) {
    cs.stop();
    Sequencer::instance().abort();
    privateContext->triggerEvent(controlStoppingDone);
  });
  
  slControlStarting.setLevelAction([&](SafetyContext*privateContext) {
    cs.start();
    AxisVector torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit};
    cs.torqueLimitation.setLimit(-torqueLimit, torqueLimit);
    privateContext->triggerEvent(controlStartingDone);
  });
  
  setEntryLevel(slControlStarting);

  exitFunction = ([&](SafetyContext* privateContext){
    Executor::stop();
  });

}
