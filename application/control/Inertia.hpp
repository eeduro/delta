#pragma once

#include <eeros/control/Block.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Output.hpp>
#include "types.hpp"
#include "Jacobian.hpp"

using namespace eeros::math;
using namespace eeros::control;

using namespace eeros;

namespace eeduro {
	namespace delta {

		class Inertia : public Block {
			public:
				Inertia(Jacobian &jacobi);
				
				virtual void run();
				
				virtual control::Input<AxisVector>& getAccelerationInput();
				virtual control::Input<AxisVector>& getTcpPosInput();
				virtual control::Input<AxisVector>& getJointPosInput();
				virtual control::Output<AxisVector>& getOut();
			
			protected:
				control::Input<AxisVector> accelerationIn;
				control::Input<AxisVector> tcpPosIn;
				control::Input<AxisVector> jointPosIn;
				control::Output<AxisVector> forceOut;
				Matrix<3,3> tcpMass;
				Matrix<3,3> motorInertia;
				Jacobian &jacobi;
		};
	}
}