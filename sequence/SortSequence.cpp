#include "SortSequence.hpp"

using namespace eeduro::delta;

SortSequence::SortSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, DeltaSafetyProperties &properties, Calibration& calibration):
	Sequence(name, caller, true),
	move("move", this, controlSys),
	detectSequence("detect sequence", controlSys, this, calibration),
	moveBlock("moveBlock", controlSys, this, calibration),
	controlSys(controlSys),
	calibration(calibration){}

int SortSequence::action() {
	std::array<int,4> blocks;
	// detect positions of all blocks
	for (int i = 0; i < 4; i++) {
		AxisVector p = {calibration.position[i].x, calibration.position[i].y, calibration.transportation_height, 0};

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
	if (!all_ok) {
		move({ 0, 0, calibration.transportation_height, 0});
		return -1;
	}
	
	while (true) {
		// find wrong block
		int wrong_block = (-1);
		int wrong_position = (-1);
		for (int i = 0; i < blocks.size(); i++) {
			wrong_position = find(blocks, i);
			if (wrong_position < 0) {
				log.error() << "cannot find block " << i;
				return -1;
			}
			if (wrong_position != i) {
				wrong_block = i;
				break;
			}
		}
		if (wrong_block < 0) break;
		
		// move block to correct position
		if (wrong_block == 0) {
			
			int correct_position = find(blocks, wrong_position);
			log.info() << "correct position: " << correct_position;
			log.info() << "wrong position: " << wrong_position;
			if (correct_position < 0) {
				log.error() << "cannot find block " << wrong_position;
				return -1;
			}
			
			log.info() << "move block from position " << correct_position << " to " << wrong_position;
			moveBlock(correct_position, wrong_position);
			std::swap(blocks[correct_position], blocks[wrong_position]);
		}
		else {
			int correct_position = wrong_block;
			int empty_position = find(blocks, 0);
			if (empty_position < 0) {
				log.error() << "cannot find block 0";
				return -1;
			}
			
			log.info() << "move block from position " << correct_position << " to " << empty_position;
			moveBlock(correct_position, empty_position);
			std::swap(blocks[correct_position], blocks[empty_position]);
			
			log.info() << "move block from position " << wrong_position << " to " << correct_position;
			moveBlock(wrong_position, correct_position);
			std::swap(blocks[wrong_position], blocks[correct_position]);
		}
	}
	move({ 0, 0, calibration.transportation_height, 0});
	log.info() << "finished sorting";
}




int SortSequence::find(const std::array<int,4> &blocks, int block) {
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i] == block)
			return i;
	}
	return -1;
}
