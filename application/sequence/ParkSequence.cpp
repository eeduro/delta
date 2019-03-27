#include "ParkSequence.hpp"


using namespace eeduro::delta;


ParkSequence::ParkSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration): 
	Sequence(name, caller, true),
	controlSys(controlSys),
	properties(properties),
	safetySys(safetySys),
	move("park move",this, controlSys),
	release("park release", this, controlSys),
	wait("wait", this),
	calibration(calibration){
	}

int ParkSequence::action()
{	
	controlSys.start();
	
	release();
	
	AxisVector p = {0, 0, calibration.transportation_height, 0};
	move(p);
	
	p[2] = calibration.position[0].zblockmin[2];
	move(p);
	wait(2);
	
	controlSys.disableAxis();
	
	safetySys.triggerEvent(properties.parkingDone);
}
