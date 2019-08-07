#pragma once

#include <vector>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "../Calibration.hpp"
#include "step/Move.hpp"
#include "step/WaitForLevel.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
	namespace delta {
		class MouseExceptionSequence : public Sequence{
			public:
				MouseExceptionSequence(std::string name, Sequence* caller, SafetySystem& safetySys, DeltaSafetyProperties& properties, DeltaControlSystem& controlSys, Calibration& calibration):
				Sequence(name, caller, true),
				controlSys(controlSys),
				safetySys(safetySys),
				properties(properties),
				move("move", this, controlSys),
				calibration(calibration),
				waitForLevel("WaitForLevel", this, safetySys){}
				
				int action(){
					move({ 0, 0, calibration.transportation_height, 0});
					controlSys.setMouseInput();
					safetySys.triggerEvent(properties.doMouseControl);
					waitForLevel(properties.slMouseControl.getLevelId());
				}

			private:
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				DeltaControlSystem& controlSys;
				Calibration& calibration;
				Move move;
				WaitForLevel waitForLevel;
		};
			
		class MouseTimeOutExceptionSequence : public Sequence{
			public:
				MouseTimeOutExceptionSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration):
				Sequence(name, caller, true),
				safetySys(safetySys),
				properties(properties),
				calibration(calibration),
				controlSys(controlSys),
				move("move", this, controlSys),
				waitForLevel("WaitForLevel", this, safetySys){}
				
				int action(){
// 					controlSys.setPathPlannerInput(); TODO
					controlSys.setCircleInput();
					move({ 0, 0, calibration.transportation_height, 0});
					safetySys.triggerEvent(properties.doAutoMoving);
					waitForLevel(properties.slAutoMoving.getLevelId());
				}
				
			private:
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				Calibration& calibration;
				DeltaControlSystem& controlSys;
				Move move;
				WaitForLevel waitForLevel;
		};
		
		class BlueButtonExceptionSequence : public Sequence{
			public:
				BlueButtonExceptionSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration):
				Sequence(name, caller, true),
				safetySys(safetySys),
				properties(properties),
				calibration(calibration),
				move("Move", this, controlSys),
				waitForLevel("WaitForLevel", this, safetySys),
				controlSys(controlSys),
				wait("Wait", this){}
				
				int action(){
					eeros::math::Vector<4> torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit, q3gearTorqueLimit };
					controlSys.torqueLimitation.setLimit(-torqueLimit, torqueLimit);
// 					controlSys.setPathPlannerInput(); TODO
					controlSys.setCircleInput();
					AxisVector p = controlSys.directKin.getOut().getSignal().getValue();
					controlSys.pathPlanner.setInitPos(p);
					
					log.warn() << p;
					p[2] = calibration.transportation_height;
					log.warn() << p;
					move(p);
					p = { 0, 0, calibration.transportation_height, 0 };
					move(p);
					safetySys.triggerEvent(properties.stopMoving);
					waitForLevel(properties.slSystemReady.getLevelId());
				}
				
			private:
				SafetySystem& safetySys;
				DeltaSafetyProperties& properties;
				DeltaControlSystem& controlSys;
				Wait wait;
				
				Move move;
				Calibration& calibration;
				WaitForLevel waitForLevel;
		};
	}
}
