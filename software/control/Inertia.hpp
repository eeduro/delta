#pragma once

#include <eeros/control/Block.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Output.hpp>
#include "types.hpp"
#include "Jacobian.hpp"

namespace eeduro {
	namespace delta {

		class Inertia : public eeros::control::Block {
			public:
				Inertia(Jacobian &jacobi);
				
				virtual void run();
				
				virtual eeros::control::Input<AxisVector>& getAccelerationInput();
				virtual eeros::control::Input<AxisVector>& getTcpPosInput();
				virtual eeros::control::Input<AxisVector>& getJointPosInput();
				virtual eeros::control::Output<AxisVector>& getOut();
			
			protected:
				eeros::control::Input<AxisVector> accelerationIn;
				eeros::control::Input<AxisVector> tcpPosIn;
				eeros::control::Input<AxisVector> jointPosIn;
				eeros::control::Output<AxisVector> forceOut;
				eeros::math::Matrix<3,3> tcpMass;
				eeros::math::Matrix<3,3> motorInertia;
				Jacobian &jacobi;
		};
	}
}