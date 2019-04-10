#pragma once

#include <eeros/sequencer/Condition.hpp>
#include "../../control/DeltaControlSystem.hpp"


using namespace eeros::sequencer;
namespace eeduro{
	namespace delta{
    
		class MoveMouseCondition : public Condition{
			public:
				MoveMouseCondition(DeltaControlSystem& controlSys) : 
					controlSys(controlSys){ /*empty*/ }

				bool validate() {
					bool retVal = false;
// 					mouseNew = controlSys.mouse.getOut().getSignal().getValue();
// 					if(controlSys.mouse.getButtonOut().getSignal().getValue()[0]
// 					|| controlSys.mouse.getButtonOut().getSignal().getValue()[1]
// 					|| controlSys.mouse.getButtonOut().getSignal().getValue()[2])
// 						retVal = true;
// 					if(mouseNew!= mouseOld)
						retVal = true; 
					return retVal;
				}
				
				void reset(){
// 					mouseOld = controlSys.mouse.getOut().getSignal().getValue();
				}

				DeltaControlSystem& controlSys;
				AxisVector mouseOld = {0.0,0.0,-0.015,0.0};
				AxisVector mouseNew = {0.0,0.0,-0.015,0.0};
		};
	}
}