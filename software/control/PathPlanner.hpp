#pragma once

#include <eeros/control/Block.hpp>
#include <eeros/control/Output.hpp>
#include <eeros/control/ConstantAccTrajectoryGenerator.hpp>
#include <mutex>
#include "types.hpp"

using namespace eeros;
using namespace eeros::control;

namespace eeduro {
	namespace delta {
		
		class PathPlanner: public Block {
			
			public:
				PathPlanner(AxisVector velMax, AxisVector accMax, double dt);
				
				virtual control::Output<AxisVector>& getPosOut();
				virtual control::Output<AxisVector>& getVelOut();
				virtual control::Output<AxisVector>& getAccOut();
				
				virtual void gotoPoint(AxisVector p);
				virtual bool posReached();
				virtual void setInitPos(AxisVector initPos);
				virtual AxisVector getLastPoint();
				
				virtual void run();
				
			protected:
				control::Output<AxisVector> posOut;
				control::Output<AxisVector> velOut;
				control::Output<AxisVector> accOut;
				std::mutex mtx;
				double tOld;
				
				ConstantAccTrajectoryGenerator<AxisVector> trajectoryGen;
				AxisVector lastPoint;
		};
	}
}