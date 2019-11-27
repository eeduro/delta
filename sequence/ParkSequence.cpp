#include "ParkSequence.hpp"

using namespace eeduro::delta;

ParkSequence::ParkSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& safetyProp): 
	Sequence(name, caller, true),
	controlSys(controlSys),
	safetyProp(safetyProp),
	safetySys(safetySys),
	move("park move",this, controlSys)
	{ }

int ParkSequence::action() {	
	move({0, 0, -0.06});
	controlSys.voltageSetPoint.setValue({0, 0, 0});	
	controlSys.voltageSwitch.switchToInput(1);
	safetySys.triggerEvent(safetyProp.parkingDone);
}
