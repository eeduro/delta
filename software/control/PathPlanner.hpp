#pragma once

#include <eeros/control/Block.hpp>
#include <eeros/control/Output.hpp>
#include <eeros/control/ConstantAccTrajectoryGenerator.hpp>
#include <mutex>
#include "types.hpp"

using namespace eeros;

namespace eeduro {
	namespace delta {
		
		class PathPlanner: public eeros::control::Block {
			
			public:
				PathPlanner(AxisVector velMax, AxisVector accMax, double dt);
				
				virtual eeros::control::Output<AxisVector>& getPosOut();
				virtual eeros::control::Output<AxisVector>& getVelOut();
				virtual eeros::control::Output<AxisVector>& getAccOut();
				
				virtual void gotoPoint(AxisVector p);
				virtual bool posReached();
				virtual void setInitPos(AxisVector initPos);
				virtual AxisVector getLastPoint();
				
				virtual void run();
				
			protected:
				eeros::control::Output<AxisVector> posOut;
				eeros::control::Output<AxisVector> velOut;
				eeros::control::Output<AxisVector> accOut;
				std::mutex mtx;
				double tOld;
				
				eeros::control::ConstantAccTrajectoryGenerator<AxisVector> trajectoryGen;
				AxisVector lastPoint;
		};
	}
}