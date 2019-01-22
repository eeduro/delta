#include "DeltaSafetyProperties.hpp"

#include <eeros/hal/HAL.hpp>
#include <eeros/safety/InputAction.hpp>
#include <eeros/safety/OutputAction.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>

#include <vector>
#include <initializer_list>

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::safety;
using namespace eeduro::delta;

DeltaSafetyProperties::DeltaSafetyProperties(DeltaControlSystem& controlSys) : 
	controlSys(controlSys),

	/*
	* ###
	* Events
	* ###
	*/
	doOff("Do off"),
	doSwInit("Do software init"),
	swInitDone("Software initialized"),
	doEmergency("Do emergency"),
	doEmergencyReset("Do emergency reset"),
	emergencyResetDone("Emergency reset done"),
	doControlStart("Do control start"),
	doControlStop("Do control stop"),
	controlStoppingDone("Control stop done"),
	controlStartingDone("Control start done"),
	doPoweringUp("Do power up"),
	poweringUpDone("Power up done"),
	doPoweringDown("Do power down"),
	poweringDownDone("Power down done"),
	doHoming("Do homing"),
	doParking("Do parking"),
	parkingDone("Parking done"),
	homingDone("Homing done"),
	doSystemReady("Do system ready"),
	doAutoMoving("Do auto moving"),
	doMouseControl("Do mouse control"),
	stopMoving("Stop moving"),
	doCalibrating("Do calibrating"),
	
	/*
	* ###
	* Levels
	* ###
	*/
	slOff("Off"),
	slSwInitializing("Software initializing"),
	slSwInitialized("Software initialized"),
	slEmergency("Emergency"),
	slResetingEmergency("Reseting emergency"),
	slControlStopping("Stop control"),
	slControlStarting("Start control"),
	slSystemOn("System on"),
	slPoweringDown("Powering down"),
	slPoweringUp("Powering up"),
	slPowerOn("Power on"),
	slHoming("Homing"),
	slAxesHomed("Axes homed"),
	slParking("Parking"),
	slParked("Parked"),
	slSystemReady("System ready"),
	slAutoMoving("Auto moving"),
	slMouseControl("Mouse control"),
	slCalibrating("Calibrating")
	{
	  
		HAL& hal = HAL::instance();

		/*
		* ###
		* Define critical outputs
		* ###
		*/
		led = hal.getLogicOutput("onBoardRed");		 //equals ledGreen of extern Led (GPIO)
		errorLed = hal.getLogicOutput("ledRed");
		
		criticalOutputs = { led , errorLed};
		
		/*
		* ###
		* Define critical inputs
		* ###
		*/
		emergencyStop = hal.getLogicInput("buttonRed");
		approval = hal.getLogicInput("buttonGreen");
		
		criticalInputs = { emergencyStop, approval};
	  
	  
		/*
		* ###
		* Add levels to Safety System
		* ###
		*/
		addLevel(slOff);
		addLevel(slEmergency);
		addLevel(slSwInitializing);
		addLevel(slSwInitialized);
		addLevel(slResetingEmergency);
		addLevel(slControlStopping);
		addLevel(slControlStarting);
		addLevel(slSystemOn);
		addLevel(slPoweringDown);
		addLevel(slPoweringUp);
		addLevel(slPowerOn);
		addLevel(slHoming);
		addLevel(slAxesHomed);
		addLevel(slParked);
		addLevel(slParking);
		addLevel(slSystemReady);
		addLevel(slAutoMoving);
		addLevel(slMouseControl);
		addLevel(slCalibrating);
	
		/*
		* ###
		* Add events to the levels
		* ###
		*/
		slOff.addEvent(doSwInit, slSwInitializing, safety::kPublicEvent);
		slSwInitializing.addEvent(swInitDone, slSwInitialized, safety::kPrivateEvent);
		slSwInitialized.addEvent(doControlStart, slControlStarting, safety::kPublicEvent);
		slEmergency.addEvent(doEmergencyReset, slResetingEmergency, safety::kPublicEvent);
		slEmergency.addEvent(doSwInit, slSwInitializing, safety::kPublicEvent);
		slEmergency.addEvent(doCalibrating, slCalibrating, eeros::safety::kPublicEvent);
		
		slResetingEmergency.addEvent(emergencyResetDone, slSystemOn, safety::kPrivateEvent);
		slResetingEmergency.addEvent(doEmergency, slEmergency, safety::kPrivateEvent);
		slControlStopping.addEvent(controlStoppingDone, slOff, safety::kPublicEvent);
		slControlStarting.addEvent(controlStartingDone, slSystemOn, safety::kPrivateEvent);
		slSystemOn.addEvent(doControlStop, slControlStopping, safety::kPublicEvent);
		slSystemOn.addEvent(doPoweringUp,slPoweringUp,safety::kPublicEvent);

		slPoweringDown.addEvent(poweringDownDone,slSystemOn,safety::kPrivateEvent);
		slPoweringUp.addEvent(poweringUpDone,slPowerOn,safety::kPrivateEvent);
		slPowerOn.addEvent(doPoweringDown,slPoweringDown,safety::kPublicEvent);
		slPowerOn.addEvent(doHoming,slHoming,safety::kPublicEvent);

		slHoming.addEvent(homingDone,slAxesHomed,safety::kPublicEvent);
		slAxesHomed.addEvent(doSystemReady,slSystemReady,safety::kPrivateEvent);
		slParking.addEvent(parkingDone,slParked,safety::kPrivateEvent);
		slParked.addEvent(doSystemReady,slSystemReady,safety::kPublicEvent);

		slSystemReady.addEvent(doAutoMoving,slAutoMoving,safety::kPublicEvent);
		slSystemReady.addEvent(doMouseControl,slMouseControl,safety::kPublicEvent);
		slAutoMoving.addEvent(doMouseControl, slMouseControl, eeros::safety::kPublicEvent);
		slMouseControl.addEvent(doAutoMoving, slAutoMoving, safety::kPublicEvent);
		slSystemReady.addEvent(doParking,slParking,kPublicEvent);

		slAutoMoving.addEvent(stopMoving,slSystemReady,safety::kPublicEvent);
		slMouseControl.addEvent(stopMoving,slSystemReady,safety::kPublicEvent);

		slCalibrating.addEvent(doSwInit, slSwInitializing, eeros::safety::kPublicEvent);

		addEventToLevelAndAbove(slSystemOn,doEmergency,slEmergency,safety::kPublicEvent);

		/*
		* ###
		* Define input states and events for all levels
		* ###
		*/
		slOff.setInputActions({ignore(emergencyStop),ignore(approval)});
		slSwInitializing.setInputActions({ignore(emergencyStop),ignore(approval)});
		slSwInitialized.setInputActions({ignore(emergencyStop),ignore(approval)});
		slEmergency.setInputActions({ignore(emergencyStop),check(approval, false, doSwInit)});
		slResetingEmergency.setInputActions({check(emergencyStop,true,doEmergency),ignore(approval)});
		slControlStopping.setInputActions({ignore(emergencyStop),ignore(approval)});
		slControlStarting.setInputActions({ignore(emergencyStop),ignore(approval)});
		slSystemOn.setInputActions({ignore(emergencyStop),check(approval,false,doPoweringUp)});
		slPoweringDown.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slPoweringUp.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slPowerOn.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slHoming.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slAxesHomed.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slParking.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slParked.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slSystemReady.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slAutoMoving.setInputActions({check(emergencyStop,false,doEmergency),check(approval, false, doMouseControl)});
		slMouseControl.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slCalibrating.setInputActions({check(emergencyStop, false, doSwInit), check(approval, false, doSystemReady)});

		/*
		* ###
		* Define output states and events for all levels
		* ###
		*/
		slOff.setOutputActions({set(led,false), set(errorLed, false)});
		slSwInitializing.setOutputActions({set(led,false), set(errorLed, false)});
		slSwInitialized.setOutputActions({set(led,true), set(errorLed, false)});
		slEmergency.setOutputActions({set(led,false), set(errorLed, true)});
		slResetingEmergency.setOutputActions({set(led,true), set(errorLed, true)});
		slControlStopping.setOutputActions({set(led,false), set(errorLed, false)});
		slControlStarting.setOutputActions({set(led,true), set(errorLed, false)});
		slSystemOn.setOutputActions({set(led,false), set(errorLed, false)});
		slPoweringDown.setOutputActions({set(led,false), set(errorLed, false)});
		slPoweringUp.setOutputActions({set(led,true), set(errorLed, false)});
		slPowerOn.setOutputActions({set(led,true), set(errorLed, false)});
		slHoming.setOutputActions({set(led,true), set(errorLed, false)});
		slAxesHomed.setOutputActions({set(led,true), set(errorLed, false)});
		slParking.setOutputActions({set(led,false), set(errorLed, false)});
		slParked.setOutputActions({set(led,false), set(errorLed, false)});
		slSystemReady.setOutputActions({set(led,true), set(errorLed, false)});
		slAutoMoving.setOutputActions({set(led,true), set(errorLed, false)});
		slMouseControl.setOutputActions({set(led,true), set(errorLed, false)});
		slCalibrating.setOutputActions({set(led,true), set(errorLed,true)});

		/*
		* ###
		* Define and add level functions
		* ###
		*/
		slOff.setLevelAction([&](SafetyContext*privateContext){
			Executor::stop();
		});

		slSwInitializing.setLevelAction([&](SafetyContext*privateContext){
			privateContext->triggerEvent(swInitDone);
		});

		slSwInitialized.setLevelAction([&](SafetyContext*privateContext){
			privateContext->triggerEvent(doControlStart);
		});

		slEmergency.setLevelAction([&](SafetyContext*privateContext){
			auto& sequencer = sequencer::Sequencer::instance();

			if(sequencer.running){
				sequencer.abort(); 
			}
			
			controlSys.voltageSetPoint.setValue({0.0,0.0,0.0,0.0});
			controlSys.voltageSwitch.switchToInput(1);
			controlSys.inputSwitch.switchToInput(1);

			if(emergencyStop->get() && (slEmergency.getNofActivations() > 1000)){
				controlSys.setVoltageForInitializing({0.0,0.0,0.0,0.0});
				
				sequencer.getSequenceByName("Calibration Sequence")->start();

				privateContext->triggerEvent(doCalibrating);
			}

		});

		slResetingEmergency.setLevelAction([&](SafetyContext*privateContext){
			exit(2);
		});
	  
		slCalibrating.setLevelAction([&](SafetyContext*privateContext){
			controlSys.emagVal.setValue(controlSys.mouse.getButtonOut().getSignal().getValue()[0]);
		});

		slControlStarting.setLevelAction([&](SafetyContext*privateContext){
			controlSys.start();
			controlSys.torqueLimitation.setLimit({-q012gearTorqueLimit,-q012gearTorqueLimit,-q012gearTorqueLimit,-q3gearTorqueLimit},{q012gearTorqueLimit,q012gearTorqueLimit,q012gearTorqueLimit,q3gearTorqueLimit});
			privateContext->triggerEvent(controlStartingDone);
		});

		slPoweringDown.setLevelAction([&](SafetyContext*privateContext){
			controlSys.disableAxis();
			privateContext->triggerEvent(poweringDownDone);
		});

		slSystemOn.setLevelAction([&](SafetyContext*privateContext){
			privateContext->triggerEvent(doPoweringUp);
		});
	  
		slPoweringUp.setLevelAction([&](SafetyContext*privateContext){
			controlSys.enableAxis();
			privateContext->triggerEvent(poweringUpDone);
		});

		slPowerOn.setLevelAction([&](SafetyContext*privateContext){
			controlSys.homed=false;
			controlSys.setVoltageForInitializing({q012InitVoltage,q012InitVoltage,q012InitVoltage,q3InitVoltage});
		
			static auto& sequencer = eeros::sequencer::Sequencer::instance();
		
			if(sequencer.getSequenceByName("Homing Sequence")->getRunningState() != eeros::sequencer::SequenceState::running){
				//controlSys.setPathPlannerInput();
				sequencer.getSequenceByName("Homing Sequence")->start();
				privateContext->triggerEvent(doHoming);
			}	
			
			

		});
    
		slHoming.setLevelAction([&](SafetyContext*privateContext){
			
		});

		slAxesHomed.setLevelAction([&](SafetyContext*privateContext){
			privateContext->triggerEvent(doSystemReady);
		});
    
		slSystemReady.setLevelAction([&](SafetyContext*privateContext){ 
			controlSys.inputSwitch.switchToInput(0);
			controlSys.voltageSwitch.switchToInput(0);
			controlSys.setPathPlannerInput();
			controlSys.pathPlanner.gotoPoint({0.0,0.0,-0.015,0.0});

			privateContext->triggerEvent(doAutoMoving);
		
		});
    
		slAutoMoving.setLevelAction([&](SafetyContext*privateContext){
			static auto& sequencer = eeros::sequencer::Sequencer::instance();
		
			if(sequencer.getSequenceByName("AutoMove Sequence")->getRunningState() != eeros::sequencer::SequenceState::running){
				controlSys.setPathPlannerInput();
				sequencer.getSequenceByName("AutoMove Sequence")->start();
			}
		});
    
		slMouseControl.setLevelAction([&](SafetyContext*privateContext){
			static auto& sequencer = eeros::sequencer::Sequencer::instance();
		
			if(sequencer.getSequenceByName("Mouse Sequence")->getRunningState() != eeros::sequencer::SequenceState::running){
				controlSys.setMouseInput();
				sequencer.getSequenceByName("Mouse Sequence")->start();
			}
		});
    
    

		/*
		* ###
		* Define entry level
		* ###
		*/
		setEntryLevel(slSwInitializing);
	
		exitFunction = ([&](SafetyContext* privateContext){
			controlSys.voltageSetPoint.setValue({0.0,0.0,0.0,0.0});
			controlSys.voltageSwitch.switchToInput(1);
			privateContext->triggerEvent(doOff);
		});
	
	}

	DeltaSafetyProperties::~DeltaSafetyProperties() { }