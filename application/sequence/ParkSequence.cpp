#include "ParkSequence.hpp"


using namespace eeduro::delta;


ParkSequence::ParkSequence(std::string name, Sequencer& seq, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration): 
	Sequence(name, seq),
	controlSys(controlSys),
	properties(properties),
	safetySys(safetySys),
	move("park move", seq, this, controlSys),
	release("park release", seq, this, controlSys),
	wait("park wait", seq, this){
	}

int ParkSequence::action()
{	
	controlSys.start();
	wait(0.5);

	controlSys.setVoltageForInitializing({-2,-2,-2,0});

	wait(2.5);
	
	controlSys.disableAxis();
	
	//controlSys.mot1.callOutputFeature<>("disable");
	
	safetySys.triggerEvent(properties.parkingDone);
}
