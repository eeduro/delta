#pragma once

#include <eeros/control/Block1i1o.hpp>
#include <eeros/math/Matrix.hpp>

#include "types.hpp"
#include "Kinematic.hpp"

using namespace eeros::math;
using namespace eeros;
using namespace eeros::control;
using namespace eeduro::delta;

namespace eeduro {
	namespace delta {
		class ReduceVector : public Block1i1o<Vector4, AxisVector> {
		public:
			ReduceVector() { }
			virtual void run() { 
				out.getSignal().setValue(in.getSignal().getValue().getSubMatrix<3, 1>(0, 0));
				out.getSignal().setTimestamp(in.getSignal().getTimestamp());
			}
		};
	}
}

