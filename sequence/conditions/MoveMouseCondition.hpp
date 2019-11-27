#pragma once

#include <eeros/sequencer/Condition.hpp>

#include "../../control/DeltaControlSystem.hpp"

using namespace eeros::sequencer;

namespace eeduro{
	namespace delta{
		class MoveMouseCondition : public Condition {
		public:
			MoveMouseCondition(DeltaControlSystem& controlSys) : controlSys(controlSys) { }
			bool validate() {
				auto pos = controlSys.redVect.getOut().getSignal().getValue();
				if (mousePosPrev != pos) {
					mousePosPrev = pos;
					return true; 
				}
				return false;
			}
			void reset() {
				mousePosPrev = controlSys.redVect.getOut().getSignal().getValue();
			}
		private:
			DeltaControlSystem& controlSys;
			AxisVector mousePosPrev;
		};
	}
}