#pragma once

#include <eeros/core/System.hpp>
#include <eeros/control/Block1o.hpp>
#include "types.hpp"

using namespace eeros;
using namespace eeros::control;

namespace eeduro {
	namespace delta {
		class CircleInput : public Block {
			public:
				CircleInput(double radius, double speed);
				
				virtual void setInitPos(double x, double y, double z, double r);
				virtual void setInitPos(Vector4 pos);
				
				virtual control::Output<AxisVector>& getOut();
				
				virtual void run();
				
			protected:
				control::Output<AxisVector> pos;
			
			double R, v;
		};
	}
}