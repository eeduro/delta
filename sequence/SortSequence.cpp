#include "SortSequence.hpp"

using namespace eeduro::delta;

SortSequence::SortSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, Calibration& cal)
    : Sequence(name, caller, true),
      move("Move in sort", this, cs),
      detectSeq("Detect sequence", this, cs, cal),
      moveBlockSeq("Move block sequence", this, cs, cal),
      controlSys(cs),
      calibration(cal) { }

int SortSequence::action() {
  std::array<int,4> blocks;
  // detect positions of all blocks
  for (int i = 0; i < blocks.size(); i++) {
    AxisVector p = {calibration.position[i].x, calibration.position[i].y, calibration.transportation_height, 0};
    move(p);
    blocks[i] = detectSeq(i);
  }
  log.warn() << "detected blocks: " << blocks[0] << " " << blocks[1] << " " << blocks[2] << " " << blocks[3] << " ";
  
  // check for invalid values and if all blocks are present
  std::array<bool,blocks.size()> present;
  present.fill(false);
  for (int i = 0; i < blocks.size(); i++) {
    if (blocks[i] < 0 || blocks[i] > 3) {
      log.error() << "index out of range (position = " << i << ", value = " << blocks[i] << ")";
      return -1;
    }
    present[blocks[i]] = true;
  }
  bool ok = true;
  for (int i = 0; i < blocks.size(); i++) {
    if (!present[i]) {
      log.error() << "missing block " << i;
      ok = false;
    }
  }
  if (!ok) {
    move({ 0, 0, calibration.transportation_height, 0});
    return -1;
  }
  
  while (true) {
    // find wrong block
    int wrongBlock = -1;
    int wrongPos = -1;
    for (int i = 0; i < blocks.size(); i++) {
      wrongPos = find(blocks, i);
      if (wrongPos < 0) {
        log.error() << "cannot find block " << i;
        return -1;
      }
      if (wrongPos != i) {
        wrongBlock = i;
        break;
      }
    }
    if (wrongBlock < 0) break;
    
    // move block to correct position
    if (wrongBlock == 0) {
      int correctPos = find(blocks, wrongPos);
      if (correctPos < 0) {
        log.error() << "cannot find block " << wrongPos;
        return -1;
      }
      moveBlockSeq(blocks[correctPos], correctPos, wrongPos);
      std::swap(blocks[correctPos], blocks[wrongPos]);
    } else {
      int correctPos = wrongBlock;
      int emptyPos = find(blocks, 0);
      if (emptyPos < 0) {
        log.error() << "cannot find block 0";
        return -1;
      }
      moveBlockSeq(blocks[correctPos], correctPos, emptyPos);
      std::swap(blocks[correctPos], blocks[emptyPos]);
      
      moveBlockSeq(blocks[wrongPos], wrongPos, correctPos);
      std::swap(blocks[wrongPos], blocks[correctPos]);
    }
  }
  move({ 0, 0, calibration.transportation_height, 0});
  return 0;
}


int SortSequence::find(const std::array<int,4> &blocks, int block) {
  for (int i = 0; i < blocks.size(); i++) {
    if (blocks[i] == block)
      return i;
  }
  return -1;
}
