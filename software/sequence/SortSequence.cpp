#include "SortSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"

using namespace eeduro::delta;
using namespace eeros::sequencer;
using namespace eeros::safety;

SortSequence::SortSequence(std::string name, eeros::sequencer::Sequencer& sequencer, eeros::sequencer::BaseSequence* caller, DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, Calibration& calibration, DeltaSafetyProperties &properties):
	Sequence(name, sequencer, caller, true),
	move("move", sequencer, this, controlSys, calibration),
	detect("detect", sequencer, this, controlSys, calibration),
	moveBlock("moveBlock", sequencer,controlSys, this, safetySys, calibration),
	controlSys(controlSys),
	safetySys(safetySys){}

int SortSequence::action() {
	std::array<int,4> blocks;
	
	// detect positions of all blocks
	for (int i = 0; i < 4; i++) {
		move(i);
		detect(i);
		blocks[i] = detect.getBlock();
		
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
	
	sortBlocks(blocks);
}


void SortSequence::sortBlocks(std::array<int,4> blocks) {
	while (true) {
		// find wrong block
		int wrong_block = (-1);
		int wrong_position = (-1);
		for (int i = 0; i < blocks.size(); i++) {
			wrong_position = find(blocks, i);
			if (wrong_position < 0) {
				log.error() << "cannot find block " << i;
				return;
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
				return;
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
				return;
			}
			
			log.info() << "move block from position " << correct_position << " to " << empty_position;
			moveBlock(correct_position, empty_position);
			std::swap(blocks[correct_position], blocks[empty_position]);
			
			log.info() << "move block from position " << wrong_position << " to " << correct_position;
			moveBlock(wrong_position, correct_position);
			std::swap(blocks[wrong_position], blocks[correct_position]);
		}
	}
	log.info() << "finished sorting";
}

int SortSequence::find(const std::array<int,4> &blocks, int block) {
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i] == block)
			return i;
	}
	return -1;
}
