#include "HomingSequence.hpp"

using namespace eeduro::delta;

HomingSequence::HomingSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties): 
	Sequence(name, caller, true),
	controlSys(controlSys),
	properties(properties),
	safetySys(safetySys),
	wait("wait", this),
	move("move", this, controlSys)
	{ }

int HomingSequence::action() {
	controlSys.voltageSetPoint.setValue({q012InitVoltage, q012InitVoltage, q012InitVoltage});	// choose fixed voltage
	controlSys.voltageSwitch.switchToInput(1);
	wait(2.5);
	controlSys.enc1.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc2.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc3.callInputFeature<>("setFqdPos", q012homingOffset);
	wait(0.1);
	controlSys.pathPlanner.setInitPos(controlSys.directKin.getOut().getSignal().getValue());
	controlSys.setPathPlannerInput();
	wait(0.1);
	controlSys.voltageSwitch.switchToInput(0);	// choose controller setpoint
	move({0, 0, tcpReady_z});
	safetySys.triggerEvent(properties.homingDone);
}
