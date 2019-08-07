#pragma once

#include <eeros/core/System.hpp>
#include <eeros/control/Block1o.hpp>
#include "types.hpp"

using namespace eeros;
using namespace eeros::control;

namespace eeduro {
	namespace delta {
		class Circle : public Block {
			public:
				Circle(double radius, double speed);
				
				virtual control::Output<AxisVector>& getOut();
				
				virtual void run();
				
			protected:
				control::Output<AxisVector> pos;
			
			double R, v;
		};
	}
}