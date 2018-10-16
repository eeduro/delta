#include "Calibration.hpp"

using namespace eeduro::delta;

Calibration::Calibration() : Calibration("/opt/eeros/etc/delta-sort.conf") { }

Calibration::Calibration(const char *path) : FileConfig(path) {
	add("p0x", position[0].x);
	add("p0y", position[0].y);
	add("p0l12", position[0].level12);
	add("p0l23", position[0].level23);
	add("p0l30", position[0].level30);
	add("p0z0", position[0].zblock[0]);
	add("p0z1", position[0].zblock[1]);
	add("p0z2", position[0].zblock[2]);
	add("p0z3", position[0].zblock[3]);
	add("p1x", position[1].x);
	add("p1y", position[1].y);
	add("p1l12", position[1].level12);
	add("p1l23", position[1].level23);
	add("p1l30", position[1].level30);
	add("p1z0", position[1].zblock[0]);
	add("p1z1", position[1].zblock[1]);
	add("p1z2", position[1].zblock[2]);
	add("p1z3", position[1].zblock[3]);
	add("p2x", position[2].x);
	add("p2y", position[2].y);
	add("p2l12", position[2].level12);
	add("p2l23", position[2].level23);
	add("p2l30", position[2].level30);
	add("p2z0", position[2].zblock[0]);
	add("p2z1", position[2].zblock[1]);
	add("p2z2", position[2].zblock[2]);
	add("p2z3", position[2].zblock[3]);
	add("p3x", position[3].x);
	add("p3y", position[3].y);
	add("p3l12", position[3].level12);
	add("p3l23", position[3].level23);
	add("p3l30", position[3].level30);
	add("p3z0", position[3].zblock[0]);
	add("p3z1", position[3].zblock[1]);
	add("p3z2", position[3].zblock[2]);
	add("p3z3", position[3].zblock[3]);
}

Calibration::~Calibration() { }


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
			position[i].zblock[j] = 0;
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
