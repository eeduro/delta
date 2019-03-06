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
	doEmergency("Do emergency"),
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
	doConfigureBlocks("Do configure blocks"),
	
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
	slPoweringDown("Powering down"),
	slPoweringUp("Powering up"),
	slHoming("Homing"),
	slAxesHomed("Axes homed"),
	slParking("Parking"),
	slParked("Parked"),
	slSystemReady("System ready"),
	slAutoMoving("Auto moving"),
	slMouseControl("Mouse control"),
	slConfigureBlocks("Configuring blocks")
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
		addLevel(slControlStopping);
		addLevel(slControlStarting);
		addLevel(slSystemOn);
		addLevel(slPoweringDown);
		addLevel(slPoweringUp);
		addLevel(slHoming);
		addLevel(slAxesHomed);
		addLevel(slSystemReady);
		addLevel(slParked);
		addLevel(slParking);
		addLevel(slConfigureBlocks);
		addLevel(slAutoMoving);
		addLevel(slMouseControl);
		
	
		/*
		* ###
		* Add events to the levels
		* ###
		*/

		slEmergency.addEvent(doControlStart, slControlStarting, safety::kPublicEvent);
		slEmergency.addEvent(doConfigureBlocks, slConfigureBlocks, eeros::safety::kPublicEvent);
		slEmergency.addEvent(doParking, slParking, eeros::safety::kPublicEvent);
		
		slControlStopping.addEvent(controlStoppingDone, slPoweringDown, safety::kPublicEvent);
		slControlStarting.addEvent(controlStartingDone, slSystemOn, safety::kPublicEvent);
		slSystemOn.addEvent(doControlStop, slControlStopping, safety::kPublicEvent);
		slSystemOn.addEvent(doPoweringUp,slPoweringUp,safety::kPublicEvent);

		slPoweringDown.addEvent(poweringDownDone,slOff,safety::kPrivateEvent);
		slPoweringUp.addEvent(doHoming,slHoming,safety::kPrivateEvent);

		slHoming.addEvent(homingDone,slAxesHomed,safety::kPublicEvent);
		slAxesHomed.addEvent(doSystemReady,slSystemReady,safety::kPrivateEvent);
		slParking.addEvent(parkingDone,slParked,safety::kPublicEvent);
		slParked.addEvent(doControlStop,slControlStopping,safety::kPublicEvent);

		slSystemReady.addEvent(doAutoMoving,slAutoMoving,safety::kPublicEvent);
		slAutoMoving.addEvent(doMouseControl, slMouseControl, eeros::safety::kPublicEvent);
		slMouseControl.addEvent(doAutoMoving, slAutoMoving, safety::kPublicEvent);
		slSystemReady.addEvent(doParking,slParking,kPublicEvent);

		slAutoMoving.addEvent(stopMoving,slSystemReady,safety::kPublicEvent);
		slMouseControl.addEvent(stopMoving,slSystemReady,safety::kPublicEvent);

		slConfigureBlocks.addEvent(doSystemReady, slSystemReady, eeros::safety::kPublicEvent);
		slConfigureBlocks.addEvent(stopMoving, slSystemReady, eeros::safety::kPublicEvent);

		addEventToLevelAndAbove(slSystemOn,doEmergency,slEmergency,safety::kPublicEvent);

		/*
		* ###
		* Define input states and events for all levels
		* ###
		*/
		slOff.setInputActions({ignore(emergencyStop),ignore(approval)});
		slEmergency.setInputActions({ignore(emergencyStop),check(approval, false, doControlStart)});
		slControlStopping.setInputActions({ignore(emergencyStop),ignore(approval)});
		slControlStarting.setInputActions({ignore(emergencyStop),ignore(approval)});
		slSystemOn.setInputActions({ignore(emergencyStop),check(approval,false,doPoweringUp)});
		slPoweringDown.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slPoweringUp.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slHoming.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slAxesHomed.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slParking.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slParked.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slSystemReady.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slAutoMoving.setInputActions({check(emergencyStop,false,doEmergency),check(approval, false, doMouseControl)});
		slMouseControl.setInputActions({check(emergencyStop,false,doEmergency),ignore(approval)});
		slConfigureBlocks.setInputActions({ignore(emergencyStop), check(approval, false, doSystemReady)});

		/*
		* ###
		* Define output states and events for all levels
		* ###
		*/
		slOff.setOutputActions({set(led,false), set(errorLed, false)});
		slEmergency.setOutputActions({set(led,false), set(errorLed, true)});
		slControlStopping.setOutputActions({set(led,false), set(errorLed, false)});
		slControlStarting.setOutputActions({set(led,true), set(errorLed, false)});
		slSystemOn.setOutputActions({set(led,false), set(errorLed, false)});
		slPoweringDown.setOutputActions({set(led,false), set(errorLed, false)});
		slPoweringUp.setOutputActions({set(led,true), set(errorLed, false)});
		slHoming.setOutputActions({set(led,true), set(errorLed, false)});
		slAxesHomed.setOutputActions({set(led,true), set(errorLed, false)});
		slParking.setOutputActions({set(led,false), set(errorLed, false)});
		slParked.setOutputActions({set(led,false), set(errorLed, false)});
		slSystemReady.setOutputActions({set(led,true), set(errorLed, false)});
		slAutoMoving.setOutputActions({set(led,true), set(errorLed, false)});
		slMouseControl.setOutputActions({set(led,true), set(errorLed, false)});
		slConfigureBlocks.setOutputActions({set(led,true), set(errorLed,true)});

		/*
		* ###
		* Define and add level functions
		* ###
		*/
		slOff.setLevelAction([&](SafetyContext*privateContext){
			Executor::stop();
		});


		slEmergency.setLevelAction([&](SafetyContext*privateContext){
			if(slEmergency.getNofActivations() == 1){  
				auto& sequencer = sequencer::Sequencer::instance();

				if(sequencer.running){
					sequencer.abort(); 
					std::cout << "SEQUENCER: " << sequencer.running << std::endl;
				}
				
				controlSys.voltageSetPoint.setValue({0.0,0.0,0.0,0.0});
				controlSys.voltageSwitch.switchToInput(1);
				controlSys.inputSwitch.switchToInput(1);
			}
			
			if(emergencyStop->get() && (slEmergency.getNofActivations() > 1000)){
				controlSys.setVoltageForInitializing({0.0,0.0,0.0,0.0});
				
				auto& sequencer = sequencer::Sequencer::instance();
				sequencer.getSequenceByName("Configure block Sequence")->start();

				privateContext->triggerEvent(doConfigureBlocks);
			}
			
			if(slEmergency.getNofActivations() == 5000){
				controlSys.enableAxis();
				privateContext->triggerEvent(doParking);
			}

		});
	  
		slConfigureBlocks.setLevelAction([&](SafetyContext*privateContext){
			controlSys.emagVal.setValue(controlSys.mouse.getButtonOut().getSignal().getValue()[0]);
		});

		slControlStarting.setLevelAction([&](SafetyContext*privateContext){
			controlSys.start();
			controlSys.torqueLimitation.setLimit({-q012gearTorqueLimit,-q012gearTorqueLimit,-q012gearTorqueLimit,-q3gearTorqueLimit},{q012gearTorqueLimit,q012gearTorqueLimit,q012gearTorqueLimit,q3gearTorqueLimit});
			privateContext->triggerEvent(controlStartingDone);
		});
		
		slControlStopping.setLevelAction([&](SafetyContext*privateContext){
			controlSys.stop();
			privateContext->triggerEvent(controlStoppingDone);
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
			privateContext->triggerEvent(doHoming);
		});
    
		slHoming.setLevelAction([&](SafetyContext*privateContext){
			if(slHoming.getNofActivations()==1){
				static auto& sequencer = eeros::sequencer::Sequencer::instance();
				sequencer.getSequenceByName("Homing Sequence")->start();
			}
		});

		slAxesHomed.setLevelAction([&](SafetyContext*privateContext){
			privateContext->triggerEvent(doSystemReady);
		});
		
		slParking.setLevelAction([&](SafetyContext*privateContext){
			if(slParking.getNofActivations()==1){
				static auto& sequencer = eeros::sequencer::Sequencer::instance();
				sequencer.getSequenceByName("Park Sequence")->start();
			}
		});
		
		slParked.setLevelAction([&](SafetyContext*privateContext){
			privateContext->triggerEvent(doControlStop);
		});
    
		slSystemReady.setLevelAction([&](SafetyContext*privateContext){ 
			controlSys.inputSwitch.switchToInput(0);
			controlSys.voltageSwitch.switchToInput(0);
			controlSys.setPathPlannerInput();
			//controlSys.pathPlanner.gotoPoint({0.0,0.0,-0.015,0.0});

			privateContext->triggerEvent(doAutoMoving);
		
		});
    
		slAutoMoving.setLevelAction([&](SafetyContext*privateContext){
			if(slAutoMoving.getNofActivations()==1){
				static auto& sequencer = eeros::sequencer::Sequencer::instance();
				controlSys.setPathPlannerInput();
				sequencer.getSequenceByName("AutoMove Sequence")->start();
			}
		});
    
		slMouseControl.setLevelAction([&](SafetyContext*privateContext){
			//if(slMouseControl.getNofActivations()==1){
			static auto& sequencer = eeros::sequencer::Sequencer::instance();
			if(sequencer.getSequenceByName("Mouse Sequence")->getRunningState() != eeros::sequencer::SequenceState::running){
				sequencer.getSequenceByName("Mouse Sequence")->start();
			}
		});
    
    

		/*
		* ###
		* Define entry level
		* ###
		*/
		setEntryLevel(slControlStarting);
	
		exitFunction = ([&](SafetyContext* privateContext){
			controlSys.voltageSetPoint.setValue({0.0,0.0,0.0,0.0});
			controlSys.voltageSwitch.switchToInput(1);
			privateContext->triggerEvent(stopMoving);
		});
	
	}

	DeltaSafetyProperties::~DeltaSafetyProperties() { }