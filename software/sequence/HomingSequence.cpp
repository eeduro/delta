#include "HomingSequence.hpp"


using namespace eeduro::delta;


HomingSequence::HomingSequence(std::string name, Sequencer& seq, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration): 
	Sequence(name, seq),
	controlSys(controlSys),
	properties(properties),
	safetySys(safetySys),
	home("home", seq, this, controlSys),
	move("move", seq, this, controlSys),
	calibration(calibration){}

int HomingSequence::action()
{
	home();
	move({0,0,calibration.transportation_height, pi/2});
	
	safetySys.triggerEvent(properties.homingDone);
}
