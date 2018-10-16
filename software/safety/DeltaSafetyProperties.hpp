#pragma once

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "../control/DeltaControlSystem.hpp"


namespace eeduro {
  namespace delta {

		   class DeltaSafetyProperties : public eeros::safety::SafetyProperties {
	
		      public:
			DeltaSafetyProperties(DeltaControlSystem& controlSys, double td);
			virtual ~DeltaSafetyProperties();
			
			/*
			 * ###
			 * Name all levels
			 * ###
			 */
			eeros::control::SafetyLevel slOff;
			eeros::control::SafetyLevel slSwInitializing;
			eeros::control::SafetyLevel slSwInitialized;
			eeros::control::SafetyLevel slEmergency;
			eeros::control::SafetyLevel slResetingEmergency;
			eeros::control::SafetyLevel slControlStopping;
			eeros::control::SafetyLevel slControlStarting;
			eeros::control::SafetyLevel slSystemOn;
			eeros::control::SafetyLevel slPoweringDown;
			eeros::control::SafetyLevel slPoweringUp;
			eeros::control::SafetyLevel slPowerOn;
			eeros::control::SafetyLevel slHomeing;
			eeros::control::SafetyLevel slAxesHomed;
			eeros::control::SafetyLevel slParking;
			eeros::control::SafetyLevel slParked;
			eeros::control::SafetyLevel slSystemReady;
			eeros::control::SafetyLevel slAutoMoving;
			eeros::control::SafetyLevel slMouseControl;
			eeros::control::SafetyLevel slCalibrating;
	  
			/*
			 * ###
			 * Define all possible events
			 * ###
			 */
			eeros::control::SafetyEvent doOff;
			eeros::control::SafetyEvent doSwInit;
			eeros::control::SafetyEvent swInitDone;
			eeros::control::SafetyEvent doEmergency;
			eeros::control::SafetyEvent doEmergencyReset;
			eeros::control::SafetyEvent emergencyResetDone;
			eeros::control::SafetyEvent doControlStart;
			eeros::control::SafetyEvent doControlStop;
			eeros::control::SafetyEvent controlStoppingDone;
			eeros::control::SafetyEvent controlStartingDone;
			eeros::control::SafetyEvent doPoweringUp;
			eeros::control::SafetyEvent poweringUpDone;
			eeros::control::SafetyEvent doPoweringDown;
			eeros::control::SafetyEvent poweringDownDone;
			eeros::control::SafetyEvent doHoming;
			eeros::control::SafetyEvent doParking;
			eeros::control::SafetyEvent parkingDone;
			eeros::control::SafetyEvent homeingDone;
			eeros::control::SafetyEvent doSystemReady;
			eeros::control::SafetyEvent doAutoMoving;
			eeros::control::SafetyEvent doMouseControl;
			eeros::control::SafetyEvent stopMoving;
			eeros::control::SafetyEvent doCalibrating;
	

	
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
	
		    private:
			double tdn;

		};
	}
}
