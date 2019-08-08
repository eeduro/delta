#include "DeltaSafetyProperties.hpp"

using namespace eeduro::delta;

DeltaSafetyProperties::DeltaSafetyProperties(DeltaControlSystem& controlSys) : 
	controlSys(controlSys),

	/*
	* ###
	* Events
	* ###
	*/
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
	doAutoMoving("Do auto moving"),
	doMouseControl("Do mouse control"),
	stopMoving("Stop moving"),
	
	/*
	* ###
	* Levels
	* ###
	*/
	slOff("Off"),
	slEmergency("Emergency"),
	slControlStopping("Stop control"),
	slControlStarting("Start control"),
	slSystemOn("System on"),
	slPoweringUp("Powering up"),
	slHoming("Homing"),
	slHomed("Homed"),
	slSystemReady("System ready"),
	slParking("Parking"),
	slParked("Parked"),
	slAutoMoving("Auto moving"),
	slMouseControl("Mouse control")
	{
	  
		HAL& hal = HAL::instance();

		/*
		* ###
		* Define critical outputs
		* ###
		*/
		greenLed = hal.getLogicOutput("ledGreen");
		errorLed = hal.getLogicOutput("ledRed");
		
		criticalOutputs = { greenLed , errorLed};
		
		/*
		* ###
		* Define critical inputs
		* ###
		*/
		emergencyStop = hal.getLogicInput("buttonRed");
		
		criticalInputs = { emergencyStop };	  
	  
		/*
		* ###
		* Add levels to Safety System
		* ###
		*/
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
		
	
		/*
		* ###
		* Add events to the levels
		* ###
		*/

		slControlStarting.addEvent(controlStartingDone, slSystemOn, kPrivateEvent);
		slSystemOn.addEvent(doPoweringUp, slPoweringUp, kPublicEvent);
		slPoweringUp.addEvent(doHoming, slHoming, kPublicEvent);
		slHoming.addEvent(homingDone, slHomed, kPublicEvent);
		slHomed.addEvent(doSystemReady, slSystemReady, kPublicEvent);
		slSystemReady.addEvent(doAutoMoving, slAutoMoving, kPublicEvent);
		slSystemReady.addEvent(doMouseControl, slMouseControl, kPublicEvent);
		slSystemReady.addEvent(doParking, slParking, kPublicEvent);
		slAutoMoving.addEvent(doMouseControl, slMouseControl, kPublicEvent);
		slMouseControl.addEvent(doAutoMoving, slAutoMoving, kPublicEvent);
		
		slEmergency.addEvent(doControlStart, slControlStarting, kPublicEvent);
		slEmergency.addEvent(doParking, slParking, kPublicEvent);
		
		slAutoMoving.addEvent(stopMoving, slSystemReady, kPublicEvent);
		slMouseControl.addEvent(stopMoving, slSystemReady, kPublicEvent);
		slParking.addEvent(parkingDone, slParked, kPublicEvent);
		slParked.addEvent(doControlStop, slControlStopping, kPublicEvent);
		slControlStopping.addEvent(controlStoppingDone, slOff, kPublicEvent);
		
		slSystemOn.addEvent(controlStoppingDone, slOff, eeros::safety::kPrivateEvent);
		
		addEventToLevelAndAbove(slSystemOn, doEmergency, slEmergency, kPublicEvent);
		
		/*
		* ###
		* Define input states and events for all levels
		* ###
		*/
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

		/*
		* ###
		* Define output states and events for all levels
		* ###
		*/
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


		/*
		* ###
		* Define and add level functions
		* ###
		*/
		slOff.setLevelAction([&](SafetyContext*privateContext){
			Executor::stop();
		});


		slEmergency.setLevelAction([&](SafetyContext*privateContext){
			controlSys.voltageSetPoint.setValue({0.0,0.0,0.0,0.0});
			controlSys.emagVal.setValue(false);
			controlSys.voltageSwitch.switchToInput(1);
			controlSys.posSwitch.switchToInput(1);
// 			controlSys.stop();
		});


		slControlStarting.setLevelAction([&](SafetyContext*privateContext){
			controlSys.start();
			controlSys.torqueLimitation.setLimit({-q012gearTorqueLimit,-q012gearTorqueLimit,-q012gearTorqueLimit,-q3gearTorqueLimit},{q012gearTorqueLimit,q012gearTorqueLimit,q012gearTorqueLimit,q3gearTorqueLimit});
			privateContext->triggerEvent(controlStartingDone);
		});
		
		slControlStopping.setLevelAction([&](SafetyContext*privateContext){
			controlSys.stop();
			auto& sequencer = Sequencer::instance();
			sequencer.abort();

			privateContext->triggerEvent(controlStoppingDone);
		});	
		
		slSystemOn.setLevelAction([&](SafetyContext*privateContext){
			if(slSystemOn.getNofActivations() >= 3000){
				privateContext->triggerEvent(controlStoppingDone);
			}
		});


		/*
		* ###
		* Define entry level
		* ###
		*/
		setEntryLevel(slControlStarting);
	
		exitFunction = ([&](SafetyContext* privateContext){
			Executor::stop();
		});
	
	}

	DeltaSafetyProperties::~DeltaSafetyProperties() { }