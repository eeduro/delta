#include "ShuffleSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace eeduro::delta;


ShuffleSequence::ShuffleSequence(std::string name, Sequencer& sequencer, BaseSequence* caller, DeltaControlSystem& controlSys, Calibration& calibration, DeltaSafetyProperties &properties) :
	Sequence(name, sequencer, caller, true),
	move("move", sequencer, this, controlSys),
	detectSequence("detect sequence", sequencer, controlSys, this, calibration),
	moveBlock("moveBlock", sequencer,controlSys, this, calibration),
	controlSys(controlSys),
	calibration(calibration){}
	

int ShuffleSequence::action() {
	std::array<int,4> blocks;
	// detect positions of all blocks
	for (int i = 0; i < 4; i++) {
		AxisVector p = {calibration.position[i].x,calibration.position[i].y, calibration.transportation_height, 0};
		
		move(p);
		
		blocks[i] = detectSequence(i);
		
	}

	{
		auto l = log.trace();
		l << "detected blocks:";
		for (int i = 0; i < 4; i++) {
			l << " " << blocks[i];
		}
	}
	
	// check for invalid values
	bool block_ok[4];
	for (int i = 0; i < 4; i++) block_ok[i] = false;
	for (int i = 0; i < 4; i++) {
		if (blocks[i] < 0 || blocks[i] > 3) {
			log.error() << "index out of range (position = " << i << ", value = " << blocks[i] << ")";
			return -1;
		}
		block_ok[blocks[i]] = true;
	}
	
	// check if all blocks are present
	bool all_ok = true;
	for (int i = 0; i < 4; i++) {
		if (!block_ok[i]) {
			log.error() << "missing block " << i;
			all_ok = false;
		}
	}
	if (!all_ok) return -1;
	
	for(int i = 0; i < blocks.size(); i++){
	  if(blocks[i] != i) return -1;	// blocks are not sorted -> start sort sequence
	}
	
	int empty_position = 0;
		
	srand(std::time(0));
	
	int blockFrom;
	int lastMoved;
	
	for(int i = 0; i < (rand()%4+2); i++){
		blockFrom = rand()%4;
	  
		if(blockFrom == empty_position || blockFrom == lastMoved) {
			blockFrom++;
			blockFrom%=4;
		}
		moveBlock(blockFrom, empty_position);
		lastMoved = empty_position;
		empty_position = blockFrom;
	}
	
	log.info() << "finished shuffling";

}

int ShuffleSequence::find(const std::array<int,4> &blocks, int block) {
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i] == block)
			return i;
	}
	return -1;
}
