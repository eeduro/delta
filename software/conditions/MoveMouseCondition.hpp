#pragma once

#include <eeros/sequencer/Condition.hpp>
#include "../control/DeltaControlSystem.hpp"

namespace eeduro{
  namespace delta{
    
    class MoveMouseCondition : public eeros::sequencer::Condition{
      public:
	MoveMouseCondition(DeltaControlSystem& controlSys) : 
	  controlSys(controlSys){ /*empty*/ }
      
	bool validate() {
	  bool retVal = false;
	  mouseNew = controlSys.mouse.getOut().getSignal().getValue();
	  if(controlSys.mouse.getButtonOut().getSignal().getValue()[0]
	    || controlSys.mouse.getButtonOut().getSignal().getValue()[1]
	    || controlSys.mouse.getButtonOut().getSignal().getValue()[2])
	  {
	    retVal = true;
	  }
	  if(mouseNew!= mouseOld){
	    retVal = true; 
	  }
	  
	  mouseOld = mouseNew;
	  
	  return retVal;
	  
	}
  
	DeltaControlSystem& controlSys;
	AxisVector mouseOld = {0.0,0.0,-0.015,0.0};
	AxisVector mouseNew = {0.0,0.0,-0.015,0.0};
  
    };

  }
}