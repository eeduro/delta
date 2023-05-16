#include "DeltaSafetyProperties.hpp"

using namespace eeduro::delta;

DeltaSafetyProperties::DeltaSafetyProperties(DeltaControlSystem& controlSys)
    : slOff("Off"),
      slEmergency("Emergency"),
      slControlStopping("Stop control system"),
      slControlStarting("Start control system"),
      slSystemOn("System on"),
      slPoweringUp("Powering up"),
      slHoming("Homing"),
      slHomed("Homed"),
      slSystemReady("System ready"),
      slParking("Parking"),
      slParked("Parked"),
      slAutoMoving("Auto moving"),
      slMouseControl("Mouse control"),

      doEmergency("Do emergency"),
      doControlStart("Do control start"),
      doControlStop("Do control stop"),
      controlStoppingDone("Control stop done"),
      controlStartingDone("Control start done"),
      doPoweringUp("Do power up"),
      doHoming("Do homing"),
      homingDone("Homing done"),
      doParking("Do parking"),
      parkingDone("Parking done"),
      doSystemReady("Do system ready"),
      doAutoMoving("Do auto moving"),
      doMouseControl("Do mouse control"),
      stopMoving("Stop moving"),

      controlSys(controlSys) {
      
  HAL& hal = HAL::instance();

  greenLed = hal.getLogicOutput("ledGreen", false);
  errorLed = hal.getLogicOutput("ledRed");	
  criticalOutputs = { greenLed , errorLed};
  
  emergencyStop = hal.getLogicInput("buttonRed");
  criticalInputs = { emergencyStop };	  
  
  addLevel(slOff);
  addLevel(slEmergency);
  addLevel(slControlStopping);
  addLevel(slControlStarting);
  addLevel(slSystemOn);
  addLevel(slPoweringUp);
  addLevel(slHoming);
  addLevel(slHomed);
  addLevel(slSystemReady);
  addLevel(slParking);
  addLevel(slParked);
  addLevel(slAutoMoving);
  addLevel(slMouseControl);
  
  slControlStarting.addEvent(controlStartingDone, slSystemOn, kPrivateEvent);
  slSystemOn.addEvent(doPoweringUp, slPoweringUp, kPublicEvent);
  slPoweringUp.addEvent(doHoming, slHoming, kPublicEvent);
  slHoming.addEvent(homingDone, slHomed, kPublicEvent);
  slHomed.addEvent(doSystemReady, slSystemReady, kPublicEvent);
  slSystemReady.addEvent(doAutoMoving, slAutoMoving, kPublicEvent);
  slSystemReady.addEvent(doParking, slParking, kPublicEvent);
  slAutoMoving.addEvent(doMouseControl, slMouseControl, kPublicEvent);
  slMouseControl.addEvent(doAutoMoving, slAutoMoving, kPublicEvent);	
  slEmergency.addEvent(doControlStart, slControlStarting, kPublicEvent);
  slAutoMoving.addEvent(stopMoving, slSystemReady, kPublicEvent);
  slMouseControl.addEvent(stopMoving, slSystemReady, kPublicEvent);
  slParking.addEvent(parkingDone, slParked, kPublicEvent);
  slParked.addEvent(doControlStop, slControlStopping, kPublicEvent);
  slControlStopping.addEvent(controlStoppingDone, slOff, kPublicEvent);
  
  addEventToLevelAndAbove(slSystemOn, doEmergency, slEmergency, kPublicEvent);
  
  slOff.setInputActions({ignore(emergencyStop)});
  slEmergency.setInputActions({ignore(emergencyStop)});
  slControlStopping.setInputActions({ignore(emergencyStop)});
  slControlStarting.setInputActions({ignore(emergencyStop)});
  slSystemOn.setInputActions({ignore(emergencyStop)});
  slPoweringUp.setInputActions({check(emergencyStop,false,doEmergency)});
  slHoming.setInputActions({check(emergencyStop,false,doEmergency)});
  slHomed.setInputActions({check(emergencyStop,false,doEmergency)});
  slSystemReady.setInputActions({check(emergencyStop,false,doEmergency)});
  slParking.setInputActions({check(emergencyStop,false,doEmergency)});
  slParked.setInputActions({check(emergencyStop,false,doEmergency)});
  slAutoMoving.setInputActions({check(emergencyStop,false,doEmergency)});
  slMouseControl.setInputActions({check(emergencyStop,false,doEmergency)});

  slOff.setOutputActions({set(greenLed,false), set(errorLed, false)});
  slEmergency.setOutputActions({set(greenLed,true), set(errorLed, true)});
  slControlStopping.setOutputActions({set(greenLed,false), set(errorLed, false)});
  slControlStarting.setOutputActions({set(greenLed,false), set(errorLed, false)});
  slSystemOn.setOutputActions({set(greenLed,false), set(errorLed, false)});
  slPoweringUp.setOutputActions({set(greenLed,false), set(errorLed, false)});
  slHoming.setOutputActions({set(greenLed,false), set(errorLed, false)});
  slHomed.setOutputActions({set(greenLed,false), set(errorLed, false)});
  slSystemReady.setOutputActions({set(greenLed,true), set(errorLed, false)});
  slParking.setOutputActions({set(greenLed,false), set(errorLed, false)});
  slParked.setOutputActions({set(greenLed,false), set(errorLed, false)});
  slAutoMoving.setOutputActions({set(greenLed,true), set(errorLed, false)});
  slMouseControl.setOutputActions({set(greenLed,true), set(errorLed, false)});

  slOff.setLevelAction([&](SafetyContext*privateContext){
    Executor::stop();
  });

  slControlStopping.setLevelAction([&](SafetyContext*privateContext) {
    controlSys.stop();
    Sequencer::instance().abort();
    privateContext->triggerEvent(controlStoppingDone);
  });
  
  slControlStarting.setLevelAction([&](SafetyContext*privateContext) {
    controlSys.start();
    AxisVector torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit, q3gearTorqueLimit};
    controlSys.torqueLimitation.setLimit(-torqueLimit, torqueLimit);
    privateContext->triggerEvent(controlStartingDone);
  });
  
  setEntryLevel(slControlStarting);

  exitFunction = ([&](SafetyContext* privateContext){
    Executor::stop();
  });

}
