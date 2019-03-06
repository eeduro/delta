#include "HomingSequence.hpp"


using namespace eeduro::delta;
using namespace eeros::sequencer;
using namespace eeros::safety;

HomingSequence::HomingSequence(std::string name, Sequencer& seq, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration): 
	Sequence(name, seq),
	controlSys(controlSys),
	properties(properties),
	safetySys(safetySys),
	raise("raise", seq, this, controlSys),
	home("home", seq, this, controlSys, calibration){}

int HomingSequence::action()
{
	raise();
	home();
	
	safetySys.triggerEvent(properties.homingDone);

	
	/*controlSys.pathPlanner.gotoPoint({0,0,0,0});
	while(!controlSys.pathPlanner.posReached()){}
	log.info() << "homing sequence, reached 0,0,0: " << controlSys.directKin.getOut().getSignal().getValue();*/
}
