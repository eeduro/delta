#include "HomingSequence.hpp"


using namespace eeduro::delta;
using namespace eeros::sequencer;
using namespace eeros::safety;

HomingSequence::HomingSequence(std::string name, Sequencer& seq, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties): 
	Sequence(name, seq),
	controlSys(controlSys),
	properties(properties),
	safetySys(safetySys),
	raise("raise", seq, this, controlSys),
	home("home", seq, this, controlSys){}

int HomingSequence::action()
{
	while(safetySys.getCurrentLevel() != properties.slHoming){}
	
	raise();
	home();
	
	safetySys.triggerEvent(properties.homingDone);
}
