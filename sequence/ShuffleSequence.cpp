#include "ShuffleSequence.hpp"

using namespace eeduro::delta;

ShuffleSequence::ShuffleSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, Calibration& cal) :
  Sequence(name, caller, true),
  move("move", this, cs),
  detectSeq("Detect sequence", this, cs, cal),
  moveBlockSeq("Move block sequence", this, cs, cal),
  controlSys(cs),
  calibration(cal) { }

int ShuffleSequence::action() {
  std::array<int,4> blocks;
  // detect positions of all blocks
  for (int i = 0; i < blocks.size(); i++) {
    AxisVector p = {calibration.position[i].x,calibration.position[i].y, calibration.transportation_height, 0};
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
  
  for (int i = 0; i < blocks.size(); i++) {
    if(blocks[i] != i) return -1;	// blocks are not sorted
  }
  
  int emptyPos = 0;
  srand(std::time(0));
  int blockFrom;
  int lastMoved;
  
  for (int i = 0; i < (rand() % 4 + 2); i++) {
    blockFrom = rand() % 4;
    if (blockFrom == emptyPos || blockFrom == lastMoved) {
      blockFrom++;
      blockFrom %= 4;
    }
    moveBlockSeq(blocks[blockFrom], blockFrom, emptyPos);
    lastMoved = emptyPos;
    emptyPos = blockFrom;
  }
  
  move({ 0, 0, calibration.transportation_height, 0});
  return(0);
}

int ShuffleSequence::find(const std::array<int,4> &blocks, int block) {
  for (int i = 0; i < blocks.size(); i++) {
    if (blocks[i] == block)
      return i;
  }
  return -1;
}
