#include "MoveBlockSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"

using namespace eeduro::delta;


MoveBlockSequence::MoveBlockSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys, BaseSequence* caller, SafetySystem& safetySys, Calibration& calibration): 
	Sequence(name, sequencer, caller, true),
	controlSys(controlSys),
	safetySys(safetySys),
	move("move", seq, this, controlSys),
	grab("grab", seq, this, controlSys),
	calibration(calibration),
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
		auto p = controlSys.pathPlanner.getLastPoint();
		p[2] = calibration.transportation_height;
		move(p);
		
		p = controlSys.pathPlanner.getLastPoint();
		p[0] = calibration.position[from].x;
		p[1] = calibration.position[from].y;
		
		if (p[3] > 1) {
			p[3] = calibration.position[from].r;
		}
		else {
			p[3] = calibration.position[from].r + pi / 2.0;
		}
		move(p);
		
		double down = calibration.position[from].zblockmax[1] + 0.001;
		p = controlSys.pathPlanner.getLastPoint();
		p[2] = down;
		move(p);
		
		double touch = calibration.position[from].zblockmax[3];
		p[2] = touch;
		move(p);
		
		grab();
	}
	if(to != -1){
		auto p = controlSys.pathPlanner.getLastPoint();
		p[2] = calibration.transportation_height;
		move(p);
		
		p = controlSys.pathPlanner.getLastPoint();
		p[0] = calibration.position[to].x;
		p[1] = calibration.position[to].y;			
		
		if (p[3] > 1) {
			p[3] = calibration.position[to].r;
		}
		else {
			p[3] = calibration.position[to].r + pi / 2.0;
		}
		move(p);
		
		double down = calibration.position[to].zblockmax[1] + 0.001;
		p = controlSys.pathPlanner.getLastPoint();
		p[2] = down;
		move(p);
		
		double touch = calibration.position[from].zblockmax[3];
		p[2] = touch;
		move(p);
		
		release();
		
		p = controlSys.pathPlanner.getLastPoint();
		p[2] = calibration.transportation_height;
		move(p);
	}
}
