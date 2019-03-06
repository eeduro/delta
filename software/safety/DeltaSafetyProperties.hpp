#pragma once

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "../control/DeltaControlSystem.hpp"


namespace eeduro {
	namespace delta {

		class DeltaSafetyProperties : public eeros::safety::SafetyProperties {
	
			public:
				DeltaSafetyProperties(DeltaControlSystem& controlSys);
				virtual ~DeltaSafetyProperties();
				
				/*
				* ###
				* Name all levels
				* ###
				*/
				eeros::control::SafetyLevel slOff;
				eeros::control::SafetyLevel slEmergency;
				eeros::control::SafetyLevel slControlStopping;
				eeros::control::SafetyLevel slControlStarting;
				eeros::control::SafetyLevel slSystemOn;
				eeros::control::SafetyLevel slPoweringDown;
				eeros::control::SafetyLevel slPoweringUp;
				eeros::control::SafetyLevel slHoming;
				eeros::control::SafetyLevel slAxesHomed;
				eeros::control::SafetyLevel slSystemReady;
				eeros::control::SafetyLevel slParking;
				eeros::control::SafetyLevel slParked;
				eeros::control::SafetyLevel slConfigureBlocks;
				eeros::control::SafetyLevel slAutoMoving;
				eeros::control::SafetyLevel slMouseControl;
		
				/*
				* ###
				* Define all possible events
				* ###
				*/
				eeros::control::SafetyEvent doEmergency;
				eeros::control::SafetyEvent doControlStart;
				eeros::control::SafetyEvent doControlStop;
				eeros::control::SafetyEvent controlStoppingDone;
				eeros::control::SafetyEvent controlStartingDone;
				eeros::control::SafetyEvent doPoweringUp;
				eeros::control::SafetyEvent poweringUpDone;
				eeros::control::SafetyEvent doPoweringDown;
				eeros::control::SafetyEvent poweringDownDone;
				eeros::control::SafetyEvent doHoming;
				eeros::control::SafetyEvent homingDone;
				eeros::control::SafetyEvent doParking;
				eeros::control::SafetyEvent parkingDone;
				eeros::control::SafetyEvent doSystemReady;
				eeros::control::SafetyEvent doAutoMoving;
				eeros::control::SafetyEvent doMouseControl;
				eeros::control::SafetyEvent stopMoving;
				eeros::control::SafetyEvent doConfigureBlocks;
	

	
			protected:
				/*
				* ###
				* critical outputs
				* ###
				*/
				eeros::hal::Output<bool>* led;
				eeros::hal::Output<bool>* errorLed;
				
				/* 
				* ###
				* critical inputs
				* ###
				*/
				eeros::hal::Input<bool>* emergencyStop;
				eeros::hal::Input<bool>* approval;
					
				DeltaControlSystem& controlSys;
		};
	}
}
