#include "DetectSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"

using namespace eeduro::delta;


DetectSequence::DetectSequence(std::string name, Sequencer& sequencer, DeltaControlSystem& controlSys, BaseSequence* caller, SafetySystem& safetySys, Calibration& calibration): 
	Sequence(name, sequencer, caller, true),
	controlSys(controlSys),
	safetySys(safetySys),
	calibration(calibration),
	move("move", seq, this, controlSys),
	wait("wait", seq, this){}

int DetectSequence::operator()(int pos)
{
	this-> position = pos;
	return start();
}

int DetectSequence::action()
{	
	double down = calibration.position[position].level12 + 0.002;
	double touch = calibration.position[position].level30 - 0.0002;

	eeros::math::Vector<4> torqueLimit{ q012gearTorqueLimit, q012gearTorqueLimit, q012gearTorqueLimit, q3gearTorqueLimit };
	eeros::math::Vector<4> torqueLimitDown = torqueLimit * 0.1;
	eeros::math::Vector<4> zero = torqueLimit * 0.01;

	auto p = controlSys.pathPlanner.getLastPoint();
	double last_z = p[2];
	p[2] = down;
	move(p);

	controlSys.torqueLimitation.setLimit(-torqueLimitDown, torqueLimitDown);
	p[2] = touch;
	move(p);

	controlSys.torqueLimitation.setLimit(-zero, zero);
	wait(0.5);
	
	double z = controlSys.directKin.getOut().getSignal().getValue()[2];
	controlSys.torqueLimitation.setLimit(-torqueLimitDown, torqueLimitDown);
	p[2] = down;
	move(p);

	controlSys.torqueLimitation.setLimit(-torqueLimit, torqueLimit);
	p[2] = last_z;
	move(p);

	int block = calibration.getBlock(position, z);
	log.trace() << "[DETECT] pos " << position << ": z = " << z << " -> block = " << block;

	blockNumber = block;

	return block;
	
}

int DetectSequence::getBlock()
{
	return blockNumber;
}

