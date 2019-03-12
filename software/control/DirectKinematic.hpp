#pragma once

#include <eeros/control/Block1i1o.hpp>
#include "types.hpp"
#include "Kinematic.hpp"

using namespace eeros::math;
using namespace eeros;
using namespace eeros::control;
using namespace eeduro::delta;

namespace eeduro {
	namespace delta {

		class DirectKinematic : public Block1i1o<AxisVector> {
			public:
				DirectKinematic(Kinematic& kin);
	
				virtual void run();
	      
			private:
				Kinematic& kinematic;
		};
	}
}

