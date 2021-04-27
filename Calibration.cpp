#include "Calibration.hpp"

using namespace eeduro::delta;

Calibration::Calibration() : Calibration("/opt/eeros/etc/delta-sort.conf") { }

Calibration::Calibration(const char *path) : FileConfig(path) {
  add("p0x", position[0].x);			// x ccordinate of position 0 
  add("p0y", position[0].y);			// x ccordinate of position y 
  add("p0l12", position[0].level12);		// medium z level between blocks 1 and 2 at position 0
  add("p0l23", position[0].level23);		// medium z level between blocks 2 and 3 at position 0
  add("p0l30", position[0].level30);		// medium z level between blocks 3 and no block 0
  add("p0z0min", position[0].zblockmin[0]);	// minimum z when no block at position 0
  add("p0z0max", position[0].zblockmax[0]);	// maximum z when no block at position 0
  add("p0z1min", position[0].zblockmin[1]);	// minimum z when block 1 at position 0
  add("p0z1max", position[0].zblockmax[1]);	// maximum z when block 1 at position 0
  add("p0z2min", position[0].zblockmin[2]);	// minimum z when block 2 at position 0
  add("p0z2max", position[0].zblockmax[2]);	// maximum z when block 2 at position 0
  add("p0z3min", position[0].zblockmin[3]);	// minimum z when block 3 at position 0
  add("p0z3max", position[0].zblockmax[3]);	// maximum z when block 3 at position 0
  add("p1x", position[1].x);
  add("p1y", position[1].y);
  add("p1l12", position[1].level12);
  add("p1l23", position[1].level23);
  add("p1l30", position[1].level30);
  add("p1z0min", position[1].zblockmin[0]);
  add("p1z0max", position[1].zblockmax[0]);
  add("p1z1min", position[1].zblockmin[1]);
  add("p1z1max", position[1].zblockmax[1]);
  add("p1z2min", position[1].zblockmin[2]);
  add("p1z2max", position[1].zblockmax[2]);
  add("p1z3min", position[1].zblockmin[3]);
  add("p1z3max", position[1].zblockmax[3]);
  add("p2x", position[2].x);
  add("p2y", position[2].y);
  add("p2l12", position[2].level12);
  add("p2l23", position[2].level23);
  add("p2l30", position[2].level30);
  add("p2z0min", position[2].zblockmin[0]);
  add("p2z0max", position[2].zblockmax[0]);
  add("p2z1min", position[2].zblockmin[1]);
  add("p2z1max", position[2].zblockmax[1]);
  add("p2z2min", position[2].zblockmin[2]);
  add("p2z2max", position[2].zblockmax[2]);
  add("p2z3min", position[2].zblockmin[3]);
  add("p2z3max", position[2].zblockmax[3]);
  add("p3x", position[3].x);
  add("p3y", position[3].y);
  add("p3l12", position[3].level12);
  add("p3l23", position[3].level23);
  add("p3l30", position[3].level30);
  add("p3z0min", position[3].zblockmin[0]);
  add("p3z0max", position[3].zblockmax[0]);
  add("p3z1min", position[3].zblockmin[1]);
  add("p3z1max", position[3].zblockmax[1]);
  add("p3z2min", position[3].zblockmin[2]);
  add("p3z2max", position[3].zblockmax[2]);
  add("p3z3min", position[3].zblockmin[3]);
  add("p3z3max", position[3].zblockmax[3]);
}

void Calibration::loadDefaults() {
  for (int i = 0; i < 4; i++) {
    position[i].x = 0;
    position[i].y = 0;
    
    if (i == 0)
      position[i].r = -0.27;
    else
      position[i].r = 0;
    
    position[i].level12 = 0.0;
    position[i].level23 = 0.0;
    position[i].level30 = 0.0;
    for (int j = 0; j < 4; j++) {
      position[i].zblockmin[j] = 0;
      position[i].zblockmax[j] = 0;
    }
  }
  transportation_height = -0.03;
}


int Calibration::getBlock(int pos, double z) {
  if (pos < 0 || pos > 3) return -1;
  if (z >= position[pos].level12) {
    return 1;
  }
  else if (z >= position[pos].level23) {
    return 2;
  }
  else if (z >= position[pos].level30) {
    return 3;
  }
  else {
    return 0;
  }
}

int Calibration::getBlock(double x, double y, double z) {
  if(x<0 && y>0)
    return getBlock(0,z);
  if(x<0 && y<0)
    return getBlock(1, z);
  if(x>0 && y<0)
    return getBlock(2,z);
  if(x>0 && y>0)
    return getBlock(3,z);
  
  return -1;
}
