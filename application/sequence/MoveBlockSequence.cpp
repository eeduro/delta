#include "MoveBlockSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"

using namespace eeduro::delta;


MoveBlockSequence::MoveBlockSequence(std::string name, DeltaControlSystem& controlSys, Sequence* caller, Calibration& calibration): 
	Sequence(name, caller, true),
	controlSys(controlSys),
	move("move", this, controlSys),
	grab("grab", this, controlSys),
	calibration(calibration),
	release("release", this, controlSys){}

int MoveBlockSequence::operator()(int from, int to)
{
	this-> to = to;
	this-> from = from;
	return start();
}

int MoveBlockSequence::action()
{
	auto p = controlSys.pathPlanner.getLastPoint();
	p[2] = calibration.transportation_height;
	move(p);
	
	p[0] = calibration.position[from].x;
	p[1] = calibration.position[from].y;
	
	if (p[3] > 1) {
		p[3] = calibration.position[from].r;
	}
	else {
		p[3] = calibration.position[from].r + pi / 2.0;
	}
	move(p);
	
	p[2] = calibration.position[from].zblockmax[1] + 0.001;
	move(p);
	
	p[2] = calibration.position[from].zblockmax[3];
	move(p);
	
	grab();

	p[2] = calibration.transportation_height;
	move(p);

	p[0] = calibration.position[to].x;
	p[1] = calibration.position[to].y;
	
	if (p[3] > 1) {
		p[3] = calibration.position[to].r;
	}
	else {
		p[3] = calibration.position[to].r + pi / 2.0;
	}
	move(p);

	p[2] = calibration.position[to].zblockmax[1] + 0.001;
	move(p);
	
	p[2] = calibration.position[from].zblockmax[3];
	move(p);
	
	release();
	
	p[2] = calibration.transportation_height;
	move(p);

}
