#include "MoveBlockSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"

using namespace eeduro::delta;
using namespace eeros::sequencer;
using namespace eeros::safety;

MoveBlockSequence::MoveBlockSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys, BaseSequence* caller, SafetySystem& safetySys, Calibration& calibration): 
	Sequence(name, sequencer, caller, true),
	controlSys(controlSys),
	safetySys(safetySys),
	move("move", seq, this, controlSys, calibration),
	up("up", seq, this, controlSys, calibration),
	down("down", seq, this, controlSys, calibration),
	grab("grab", seq, this, controlSys),
	release("release", seq, this, controlSys){}

int MoveBlockSequence::operator()(int from, int to)
{
	this-> to = to;
	this-> from = from;
	return start();
}

int MoveBlockSequence::action()
{
	if(from != -1){
		up();
		move(from);
		down(from);
		grab();
	}
	if(to != -1){
		up();
		move(to);
		down(to);
		release();
		up();
	}
}
