#pragma once

#include <eeros/control/Block.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Output.hpp>
#include <eeros/math/Matrix.hpp>

#include "types.hpp"
#include "Jacobian.hpp"

using namespace eeros;
using namespace eeros::math;
using namespace eeros::control;

namespace eeduro {
	namespace delta {
		class Jacobi : public Block {
			public:
				Jacobi(Jacobian& j);
			
				virtual void run();
				
				virtual control::Input<AxisVector>& getForceInput();
				virtual control::Input<AxisVector>& getTcpPosInput();
				virtual control::Input<AxisVector>& getJointPosInput();
				virtual control::Output<AxisVector>& getOut();
			
			protected:
				control::Input<AxisVector> forceIn;
				control::Input<AxisVector> tcpPosIn;
				control::Input<AxisVector> jointPosIn;
				control::Output<AxisVector> torqueOut;
			
			private:
				Jacobian& jacobi;
		};
	}
}