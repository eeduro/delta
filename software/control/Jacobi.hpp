#pragma once

#include <eeros/control/Block.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Output.hpp>
#include "types.hpp"
#include "Jacobian.hpp"

namespace eeduro {
	namespace delta {

		class Jacobi : public eeros::control::Block {
			public:
				Jacobi(Jacobian& j);
			
				virtual void run();
				
				virtual eeros::control::Input<AxisVector>& getForceInput();
				virtual eeros::control::Input<AxisVector>& getTcpPosInput();
				virtual eeros::control::Input<AxisVector>& getJointPosInput();
				virtual eeros::control::Output<AxisVector>& getOut();
			
			protected:
				eeros::control::Input<AxisVector> forceIn;
				eeros::control::Input<AxisVector> tcpPosIn;
				eeros::control::Input<AxisVector> jointPosIn;
				eeros::control::Output<AxisVector> torqueOut;
			
			private:
				Jacobian& jacobi;
		};
	}
}