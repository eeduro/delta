#include "ParkSequence.hpp"


using namespace eeduro::delta;


ParkSequence::ParkSequence(std::string name, Sequencer& seq, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration): 
	Sequence(name, seq),
	controlSys(controlSys),
	properties(properties),
	safetySys(safetySys),
	move("park move", seq, this, controlSys),
	release("park release", seq, this, controlSys),
	calibration(calibration){
	}

int ParkSequence::action()
{	
	controlSys.start();
	
	release();
	
	AxisVector p = {0, 0, calibration.transportation_height, 0};
	move(p);
	
	p[2] = -0.05;
	move(p);
	wait(2);
	
	controlSys.disableAxis();
	
	safetySys.triggerEvent(properties.parkingDone);
}
