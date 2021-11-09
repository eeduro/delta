#include "DeltaSafetyProperties.hpp"

using namespace eeduro::delta;

DeltaSafetyProperties::DeltaSafetyProperties(DeltaControlSystem& controlSys) 
    : controlSys(controlSys),

      doEmergency("Do emergency"),
      doControlStart("Do control start"),
      doControlStop("Do control stop"),
      controlStoppingDone("Control stop done"),
      controlStartingDone("Control start done"),
      doPoweringUp("Do power up"),
      doHoming("Do homing"),
      doParking("Do parking"),
      parkingDone("Parking done"),
      homingDone("Homing done"),
      doSystemReady("Do system ready"),
      doMoving("Do moving"),
      doMouseControl("Do mouse control"),
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
      slParking("Parking"),
      slParked("Parked"),
      slMoving("Moving"),
      slMouseControl("Mouse control") {
      
  HAL& hal = HAL::instance();

  greenLed = hal.getLogicOutput("ledGreen");
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
  addLevel(slMoving);
  addLevel(slMouseControl);
  
  slControlStarting.addEvent(controlStartingDone, slSystemOn, kPrivateEvent);
  slSystemOn.addEvent(doPoweringUp, slPoweringUp, kPublicEvent);
  slPoweringUp.addEvent(doHoming, slHoming, kPublicEvent);
  slHoming.addEvent(homingDone, slHomed, kPublicEvent);
  slHomed.addEvent(doSystemReady, slSystemReady, kPublicEvent);
  slSystemReady.addEvent(doMoving, slMoving, kPublicEvent);
  slSystemReady.addEvent(doParking, slParking, kPublicEvent);
  slMoving.addEvent(doMouseControl, slMouseControl, kPublicEvent);
  slMouseControl.addEvent(doMoving, slMoving, kPublicEvent);	
  slEmergency.addEvent(doControlStart, slControlStarting, kPublicEvent);
  slMoving.addEvent(stopMoving, slSystemReady, kPublicEvent);
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
  slMoving.setInputActions({check(emergencyStop,false,doEmergency)});
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
  slMoving.setOutputActions({set(greenLed,true), set(errorLed, false)});
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
    AxisVector torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit};
    controlSys.torqueLimitation.setLimit(-torqueLimit, torqueLimit);
    privateContext->triggerEvent(controlStartingDone);
  });
  
  setEntryLevel(slControlStarting);

  exitFunction = ([&](SafetyContext* privateContext){
    Executor::stop();
  });

}
