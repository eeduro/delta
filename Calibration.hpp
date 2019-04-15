#pragma once

#include <eeros/config/FileConfig.hpp>

using namespace eeros::config;

namespace eeduro {
	namespace delta {
		class Calibration : public FileConfig {
			public:
				Calibration();
				Calibration(const char *path);
				virtual ~Calibration();
				
				virtual void loadDefaults();
				
				int getBlock(int pos, double z);
				
				int getBlock(double x, double y, double z);
				
				struct {
					double x;
					double y;
					double r;
					double level12;
					double level23;
					double level30;
					double zblockmin[4];
					double zblockmax[4];
				} position[4];
				double transportation_height;
		};
	}
}